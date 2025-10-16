// GraduationProjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GraduationProject.h"
#include "GraduationProjectDlg.h"
#include "afxdialogex.h"
#include "CCryptocurrency.h"
#include "CDatabaseClass.h"
#include "CIniOIClass.h"
#include "CUdpClient.h"
#include "CFileEncryptor.h"
#include <string>
#include "CFileReadTool.h"
#include "CRegistryHelper.h"
#include "CMachineID.h"
#include "CLogin.h"
#include "CTreeControlUser.h"
#include "CPictureShowDlg.h"
#include "CPictureCtrl.h"
#include "CSystemAllInfoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct CtlInfo
{
	CWnd* pWnd;
	CRect rect;    // 初始位置大小
	int id;        // 控件ID，可选
};
CArray<CtlInfo, CtlInfo&> m_arrCtlInfo;
CRect m_rectDlg;  // 对话框初始大小

static CString GetExePath()
{
	CString strIniPath;
	GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
	strIniPath.ReleaseBuffer();
	int pos = strIniPath.ReverseFind(_T('\\'));
	strIniPath = strIniPath.Left(pos + 1) + _T("config.ini");
	return strIniPath;
}

CCryptocurrency* cryptocurrency = new CCryptocurrency();
CDatabaseClass* databaseclass = new CDatabaseClass();
CIniOIClass* inioiclass_main = new CIniOIClass(GetExePath());
CUdpClient* udpclient = new CUdpClient();
//CInitializationClass* initialization = new CInitializationClass();
CFileEncryptor* fileencryptor = new CFileEncryptor();
CRegistryHelper* registry = new CRegistryHelper();
CMachineID* machineid = new CMachineID();
CPictureCtrl* picturectrl = new CPictureCtrl();
CSystemAllInfoDlg* systemallinfodlg = new CSystemAllInfoDlg();

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_ContentOpinion;
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ContentOpinion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGraduationProjectDlg 对话框



CGraduationProjectDlg::CGraduationProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRADUATIONPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraduationProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_message_list);
	DDX_Control(pDX, IDC_LIST2, m_user_list);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_wndShellTree);
}

BEGIN_MESSAGE_MAP(CGraduationProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGraduationProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CGraduationProjectDlg::OnBnClickedButton4)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CGraduationProjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CGraduationProjectDlg::OnBnClickedButton5)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CGraduationProjectDlg::OnTvnSelchangedMfcshelltree1)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON6, &CGraduationProjectDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CGraduationProjectDlg 消息处理程序

BOOL CGraduationProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CLogin dlg;
	dlg.DoModal();
	ConfigureProgramInformation();
	if (!CMachineID::InitCOM())
	{
		AfxMessageBox(_T("COM 初始化失败"));
		return FALSE;
	}



	//初始化列表控件
	// 设置样式：全行选择 + 栅格线
	m_message_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_message_list.InsertColumn(0, _T("图标"), LVCFMT_CENTER, 50);
	m_message_list.InsertColumn(1, _T("时间"), LVCFMT_CENTER, 200);
	m_message_list.InsertColumn(2, _T("文件名"), LVCFMT_CENTER, 300);
	m_message_list.InsertColumn(3, _T("文件大小"), LVCFMT_CENTER, 100);
	m_message_list.InsertColumn(4, _T("文件类型"), LVCFMT_CENTER, 100);
	m_message_list.InsertColumn(5, _T("创建日期"), LVCFMT_CENTER, 200);
	m_message_list.InsertColumn(6, _T("完整路径"), LVCFMT_CENTER, 450);

	m_imageList.Create(32, 32, ILC_COLOR32, 0, 1); // 小图标
	m_message_list.SetImageList(&m_imageList, LVSIL_SMALL);


	// 保存对话框客户区初始大小
	GetClientRect(&m_rectDlg);

	// 遍历所有子控件
	CWnd* pWndChild = GetWindow(GW_CHILD);
	while (pWndChild)
	{
		CtlInfo info;
		info.pWnd = pWndChild;
		info.id = pWndChild->GetDlgCtrlID();
		pWndChild->GetWindowRect(&info.rect);
		ScreenToClient(&info.rect); // 转成客户区坐标
		m_arrCtlInfo.Add(info);

		pWndChild = pWndChild->GetNextWindow();
	}



	colRatios[0] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("image"), 0);
	colRatios[1] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("time"), 0);
	colRatios[2] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("file_name"), 0);
	colRatios[3] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("file_size"), 0);
	colRatios[4] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("file_type"), 0);
	colRatios[5] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("file_time"), 0);
	colRatios[6] = inioiclass_main->ReadInt(_T("ColumnWidthConfig"), _T("all_file_path"), 0);

	colRatiosB[0] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("ID"), 0);
	colRatiosB[1] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("name"), 0);
	colRatiosB[2] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("info"), 0);
	colRatiosB[3] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("Permission"), 0);
	colRatiosB[4] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("Launch_time"), 0);
	colRatiosB[5] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("addr"), 0);
	colRatiosB[6] = inioiclass_main->ReadInt(_T("UserListColumnWidthConfig"), _T("Other"), 0);

	// TODO: 在此添加额外的初始化代码


	m_user_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_user_list.InsertColumn(0, _T("用户ID"), LVCFMT_CENTER, 150);
	m_user_list.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, 150);
	m_user_list.InsertColumn(2, _T("用户信息"), LVCFMT_CENTER, 200);
	m_user_list.InsertColumn(3, _T("权限"), LVCFMT_CENTER, 100);
	m_user_list.InsertColumn(4, _T("上线时间"), LVCFMT_CENTER, 200);
	m_user_list.InsertColumn(5, _T("地址"), LVCFMT_CENTER, 200);
	m_user_list.InsertColumn(6, _T("其他"), LVCFMT_CENTER, 100);





	//加载用户列表数据（使用线程）

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGraduationProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGraduationProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGraduationProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGraduationProjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	picturectrl->ShowImage("C:\\Users\\kangs\\Pictures\\Screenshots\\777.png");
}


void CGraduationProjectDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	picturectrl->DownloadImageFromURL("https://s1.aigei.com/src/img/jpg/34/342e6d84d3b1424aa269bc5d8220c675.jpg?download/%E7%94%B5%E8%84%91%E5%8A%A8%E6%BC%AB%E5%A3%81%E7%BA%B8_%E7%88%B1%E7%BB%99%E7%BD%91_aigei_com.jpg&e=1760529960&token=P7S2Xpzfz11vAkASLTkfHN7Fw-oOZBecqeJaxypL:xyaZm7XFSsoaaB5KR6um8gXAjho=&s=978009&t=04044fd953d14a74996c01f0d3687eb4", ".\\1.png");
	picturectrl->ShowImage(".\\1.png");
}


BOOL CGraduationProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		// 屏蔽回车键
		if (pMsg->wParam == VK_RETURN)
			return TRUE;

		// 屏蔽 ESC 键
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;

		// 屏蔽 Alt+F4
		if (pMsg->wParam == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000))
			return TRUE; // 拦截 Alt+F4，不关闭窗口
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGraduationProjectDlg::ConfigureProgramInformation()
{
	// 读取程序名称
	CString strAppName = inioiclass_main->ReadString(_T("EXEINFO"), _T("ExeName"), _T("默认程序名"));
	if (!strAppName.IsEmpty())
	{
		SetWindowText(strAppName);   // 设置窗口标题
	}

	// 读取图标路径
	CString strIconPath = inioiclass_main->ReadString(_T("EXEINFO"), _T("ExeIcon"), _T(""));
	if (!strIconPath.IsEmpty())
	{
		HICON hIcon = (HICON)LoadImage(
			NULL,
			strIconPath,
			IMAGE_ICON,
			32, 32,                          // 大图标
			LR_LOADFROMFILE
		);

		if (hIcon)
		{
			SetIcon(hIcon, TRUE);   // 设置大图标
			SetIcon(hIcon, FALSE);  // 设置小图标
		}
	}
}
void CGraduationProjectDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox(_T("你确定要退出吗？"), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		// 用户点击了“否”
		// 不保存，直接关闭
		return;
	}
	CDialogEx::OnClose();
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString opinion;
	m_ContentOpinion.GetWindowText(opinion);
	if(opinion.IsEmpty())
	{
		AfxMessageBox("反馈内容不能为空！");
		return;
	}
	AfxMessageBox("反馈成功！");
	CDialogEx::OnOK();
}

void CGraduationProjectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CTreeControlUser dlg;
	dlg.DoModal();
}

CString strPath;
void CGraduationProjectDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(strPath.IsEmpty())
	{
		AfxMessageBox("请先选择文件夹！");
		return;
	}
	GetFilesInFolder(strPath);
}


void CGraduationProjectDlg::OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_wndShellTree.GetSelectedItem();
	if (hItem != NULL)
	{
		m_wndShellTree.GetItemPath(strPath, hItem); // 修正：传入CString&和HTREEITEM
	}
	*pResult = 0;
}



void CGraduationProjectDlg::GetFilesInFolder(const CString& folderPath)
{
	m_message_list.DeleteAllItems(); // 清空列表

	CFileFind finder;
	CString searchPath = folderPath + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots()) continue; // 跳过 "." ".."
		if (finder.IsDirectory()) continue; // 跳过文件夹

		CString fileName = finder.GetFileName();
		CString fullPath = finder.GetFilePath();

		// 文件大小
		ULONGLONG fileSize = finder.GetLength();
		CString strSize;
		strSize.Format(_T("%llu KB"), fileSize / 1024);

		// 文件类型（扩展名）
		CString fileType = PathFindExtension(fileName); // 带点
		if (!fileType.IsEmpty() && fileType[0] == _T('.'))
			fileType = fileType.Mid(1);

		// 创建日期
		CTime createTime;
		finder.GetCreationTime(createTime);
		CString strCreateTime = createTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		// 获取文件小图标
		SHFILEINFO sfi;
		HIMAGELIST hImgList = (HIMAGELIST)SHGetFileInfo(fullPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
			SHGFI_SMALLICON | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

		int imageIndex = m_imageList.Add(sfi.hIcon); // 添加到图像列表
		::DestroyIcon(sfi.hIcon); // 释放临时图标

		// 插入到列表控件
		CTime currentTime = CTime::GetCurrentTime();  // 获取系统当前时间
		CString strTime = currentTime.Format(_T("%Y-%m-%d %H:%M:%S")); // 格式化
		int nIndex = m_message_list.InsertItem(m_message_list.GetItemCount(), _T(""), imageIndex); // 第一列显示图标
		m_message_list.SetItemText(nIndex, 1, strTime);// 第一列显示图标
		m_message_list.SetItemText(nIndex, 2, fileName);
		m_message_list.SetItemText(nIndex, 3, strSize);
		m_message_list.SetItemText(nIndex, 4, fileType);
		m_message_list.SetItemText(nIndex, 5, strCreateTime);
		m_message_list.SetItemText(nIndex, 6, fullPath);
	}
}


void CGraduationProjectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_arrCtlInfo.GetSize() == 0) return; // 初始还没创建

	float fWidthRatio = (float)cx / m_rectDlg.Width();
	float fHeightRatio = (float)cy / m_rectDlg.Height();

	for (int i = 0; i < m_arrCtlInfo.GetSize(); i++)
	{
		CtlInfo& info = m_arrCtlInfo[i];
		CRect newRect;
		newRect.left = (LONG)(info.rect.left * fWidthRatio);
		newRect.top = (LONG)(info.rect.top * fHeightRatio);
		newRect.right = (LONG)(info.rect.right * fWidthRatio);
		newRect.bottom = (LONG)(info.rect.bottom * fHeightRatio);

		// 调整控件位置和大小
		info.pWnd->MoveWindow(newRect);

		// 如果是列表控件，调整列宽
		if (info.pWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)))
		{
			CListCtrl* pList = (CListCtrl*)info.pWnd;
			int nCtrlID = pList->GetDlgCtrlID();  // 获取控件ID

			int* ratios = nullptr;

			if (nCtrlID == IDC_LIST1)        // 第一个列表
				ratios = colRatios;
			else if (nCtrlID == IDC_LIST2)   // 第二个列表
				ratios = colRatiosB;
			else
				return;

			int nCols = pList->GetHeaderCtrl()->GetItemCount();
			if (nCols > 0)
			{
				int totalWidth = newRect.Width();

				for (int col = 0; col < nCols; col++)
				{
					int colWidth = totalWidth * ratios[col] / 100;
					pList->SetColumnWidth(col, colWidth);
				}
			}
		}
	}
}

void CGraduationProjectDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 设置最小宽度和高度
	lpMMI->ptMinTrackSize.x = 1000; // 最小宽度
	lpMMI->ptMinTrackSize.y = 800; // 最小高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CGraduationProjectDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	systemallinfodlg->DoModal();
}

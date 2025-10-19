// CLogin.cpp: 实现文件
//

#include "pch.h"
#include "afx.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CLogin.h"
#include "CCryptocurrency.h"
#include "CDatabaseClass.h"
#include "CRegistryHelper.h"
#include "CRegistrationDlg.h"
#include "CConfigDlg.h"
#include "CIniOIClass.h"
#include "CToken.h"

struct UserData
{
	CString userID;
	CString userName;
	CString userPassword;
	CString userInfo;
	CString userPhone;
	CString userEmali;
};


static CString GetExePath()
{
	CString strIniPath;
	GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
	strIniPath.ReleaseBuffer();
	int pos = strIniPath.ReverseFind(_T('\\'));
	strIniPath = strIniPath.Left(pos + 1) + _T("config.ini");
	return strIniPath;
}


CCryptocurrency* cryptocurrency1 = new CCryptocurrency();
CDatabaseClass* databaseclass1 = new CDatabaseClass();
CRegistryHelper* registryhelper = new CRegistryHelper();
CToken* tokenclass = new CToken();
CIniOIClass* inioiclass = new CIniOIClass(GetExePath());

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_user_name);
	DDX_Control(pDX, IDC_EDIT3, m_password);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON8, &CLogin::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogin::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON3, &CLogin::OnBnClickedButton3)
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	LoginFun();
}

void CLogin::LoginFun()
{
	UserData database;
	CString only_id;
	if (registryhelper->CreateKey(HKEY_CURRENT_USER, "UserOnlyID"))
	{
		std::string id;
		if (registryhelper->GetStringValue("id", id))
		{
			only_id = cryptocurrency1->AESDecrypt(id.c_str(), "200958ks");
			//MessageBox(only_id);
			registryhelper->CloseKey();
		}
		else
		{
			MessageBox("失败！");
			return;
		}
	}
	std::vector<MySQLRow> results;
	CString user, password;
	m_user_name.GetWindowText(user);
	m_password.GetWindowText(password);
	if (user.IsEmpty() || password.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	INI_FILE_INFO data_base;
	data_base = inioiclass->GetINI_File_DataBase_Info();
	int port_tmp = 0;{};
	data_base.PORT.Format("%d", port_tmp);
	if (!databaseclass1->Connect(data_base.IP, data_base.USER, data_base.PASSWORD, data_base.DBNAME, port_tmp))
	{
		MessageBox("数据库连接失败!");
		return;
	}
	CString sql;
	sql.Format("SELECT userID, userName, userPassword FROM userdata WHERE userID = '%s'", cryptocurrency1->AESEncrypt(only_id, "200958ks"));
	//MessageBox(sql);
	if (databaseclass1->ExecuteQuery(sql, results))
	{
		if (!results.size() == 0)
		{
			for (size_t i = 0; i < results.size(); i++)
			{
				database.userID = results[i].columns[0];
				database.userName = results[i].columns[1];
				database.userPassword = results[i].columns[2];
				if (database.userID == cryptocurrency1->AESEncrypt(only_id, "200958ks") && database.userName == user && database.userPassword == cryptocurrency1->GetMD5Hash(password))
				{
					//MessageBox("登录成功!");
					//CString nowtime = tokenclass->GetSystemTime();
					//MessageBox(tokenclass->AddSeconds(nowtime, 360));
					EndDialog(0);
				}
				else
				{
					MessageBox("登录失败!");
					return;
				}
			}
		}
		else
		{
			MessageBox("没有该用户");
			return;
		}
	}
	else
	{
		MessageBox("系统错误!");
	}
}

void CLogin::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CRegistrationDlg dlg;
	dlg.DoModal();
}


void CLogin::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if ((configdlg2 && ::IsWindow(configdlg2->GetSafeHwnd())))
	{
		return;
	}
	// 设置弹出窗口大小
	int nWidth = 975;
	int nHeight = 750;
	 // 获取屏幕尺寸
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 计算左上角坐标，实现居中
	int nX = (nScreenWidth - nWidth) / 2;
	int nY = (nScreenHeight - nHeight) / 2;


	// 创建并显示弹出窗口
	configdlg2 = new CConfigDlg();
	configdlg2->Create(IDD_DIALOG4, NULL); // 父窗口可为主对话框
	configdlg2->SetParent(NULL);
	configdlg2->SetChildMode(0);


	// 移动窗口并刷新样式，同时设置为置顶
	configdlg2->SetWindowPos(&CWnd::wndTopMost, nX, nY, nWidth, nHeight,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	// 设置窗口标题
	configdlg2->SetWindowText(_T("连接配置"));
	
	//MessageBox("请在配置完成后，点击保存按钮保存配置，然后关闭此窗口！");
	//etDlgItem(IDC_TAB_MAIN)->EnableWindow(FALSE); // 禁用 Tab 控件，模拟模态效果
	configdlg2->SetOwner(this);
}


void CLogin::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int a = AfxMessageBox("你确定要退出吗？", MB_YESNO | MB_ICONQUESTION);
	if (a == IDNO)
	{
		// 用户点击了“否”
		// 不保存，直接关闭
		return;
	}
	else if(a == IDYES)
	{
		// 用户点击了“是”
		// 保存设置，然后关闭
		// 在这里添加保存设置的代码
		exit(0);
	}
	CDialogEx::OnClose();
}


BOOL CLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		// 屏蔽回车键
		if (pMsg->wParam == VK_RETURN)
		{
			LoginFun();
			return 0;
		}

		// 屏蔽 ESC 键
		if (pMsg->wParam == VK_ESCAPE)
		{
			return 1;
		}

		// 屏蔽 Alt+F4
		if (pMsg->message == WM_SYSKEYDOWN)  // Alt 系列组合键会走这里
		{
			if (pMsg->wParam == VK_F4)       // Alt+F4
			{
				// 屏蔽 Alt+F4，啥都不做
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogin::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnSysCommand(nID, lParam);
}

BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	configdlg2 = nullptr;
	ConfigureProgramInformation();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLogin::ConfigureProgramInformation()
{	
	// 读取程序名称
	CString strAppName = inioiclass->ReadString(_T("EXEINFO"), _T("ExeName"), _T("默认程序名"));
	if (!strAppName.IsEmpty())
	{
		SetWindowText(strAppName);   // 设置窗口标题
	}

	// 读取图标路径
	CString strIconPath = inioiclass->ReadString(_T("EXEINFO"), _T("ExeIcon"), _T(""));
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

void CLogin::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CSystemAllInfoDlg dlg;
	dlg.DoModal();
}

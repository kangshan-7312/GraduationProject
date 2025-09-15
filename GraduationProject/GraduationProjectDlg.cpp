
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


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CCryptocurrency* cryptocurrency = new CCryptocurrency();
CDatabaseClass* databaseclass = new CDatabaseClass();
CIniOIClass inioiclass(".\\config.ini");
CUdpClient* udpclient = new CUdpClient();
//CInitializationClass* initialization = new CInitializationClass();
CFileEncryptor* fileencryptor = new CFileEncryptor();
CRegistryHelper* registry = new CRegistryHelper();
CMachineID* machineid = new CMachineID();

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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
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
}

BEGIN_MESSAGE_MAP(CGraduationProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGraduationProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CGraduationProjectDlg::OnBnClickedButton4)
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
	if (!CMachineID::InitCOM())
	{
		AfxMessageBox(_T("COM 初始化失败"));
		return FALSE;
	}
	// TODO: 在此添加额外的初始化代码
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
}


void CGraduationProjectDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CLogin dlg;
	dlg.DoModal();
}

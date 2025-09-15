// CLogin.cpp: 实现文件
//

#include "pch.h"
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

CCryptocurrency* cryptocurrency1 = new CCryptocurrency();
CDatabaseClass* databaseclass1 = new CDatabaseClass();
CRegistryHelper* registryhelper = new CRegistryHelper();
CToken* tokenclass = new CToken();
CIniOIClass* inioiclass = new CIniOIClass(".\\Config.ini");
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
					MessageBox("登录成功!");
					CString nowtime = tokenclass->GetSystemTime();
					MessageBox(tokenclass->AddSeconds(nowtime, 360));
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
	CConfigDlg dlg;
	dlg.DoModal();
}


void CLogin::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	exit(0);
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
		if (pMsg->wParam == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000))
			return TRUE; // 拦截 Alt+F4，不关闭窗口
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogin::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox(_T("你确定要退出吗？"), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		// 用户点击了“否”
		// 不保存，直接关闭
		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

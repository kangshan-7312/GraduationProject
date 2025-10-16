// CRegistrationDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CRegistrationDlg.h"
#include "CCryptocurrency.h"
#include "CDatabaseClass.h"
#include "CRegistryHelper.h"
#include "CMachineID.h"
#include "CIniOIClass.h"
#include <regex>
#include "string"


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

CCryptocurrency* cryptocurrency2 = new CCryptocurrency();
CDatabaseClass* databaseclass2 = new CDatabaseClass();
CRegistryHelper* registryhelper2 = new CRegistryHelper();
CMachineID* machineid1 = new CMachineID();
CIniOIClass* inioiclass3 = new CIniOIClass(GetExePath());


// CRegistrationDlg 对话框

IMPLEMENT_DYNAMIC(CRegistrationDlg, CDialogEx)

CRegistrationDlg::CRegistrationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CRegistrationDlg::~CRegistrationDlg()
{
}

void CRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_user);
	DDX_Control(pDX, IDC_EDIT4, m_password);
	DDX_Control(pDX, IDC_EDIT5, m_info);
	DDX_Control(pDX, IDC_EDIT6, m_phone);
	DDX_Control(pDX, IDC_EDIT7, m_eamli);
}


BEGIN_MESSAGE_MAP(CRegistrationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegistrationDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CRegistrationDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CRegistrationDlg 消息处理程序


void CRegistrationDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<MySQLRow> results;
	UserData database;
	CString only_id;
	CString userName, userPassword, userInfo, userPhone, userEmali;
	m_user.GetWindowText(userName);
	m_password.GetWindowText(userPassword);
	m_info.GetWindowText(userInfo);
	m_phone.GetWindowText(userPhone);
	m_eamli.GetWindowText(userEmali);
	if (userPassword.IsEmpty() || userName.IsEmpty() || userPhone.IsEmpty() || userInfo.IsEmpty() || userEmali.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	if (!IsValidPassword(userPassword))
	{
		MessageBox("密码不合规，至少8位，至少一个大写字母和一个小写字母一个特殊符号!");
		return;
	}
	if (!IsValidChinaPhone(userPhone))
	{
		MessageBox("电话号码不合规，请按照“+861xxxx.......格式”");
		return;
	}
	if (!IsValidEmail(userEmali))
	{
		MessageBox("电子邮箱不合规，请重新输入!");
		return;
	}
	only_id = machineid1->GetUniqueID();
	if (only_id.IsEmpty())
	{
		MessageBox("系统发生错误!");
		return;
	}
	if (registryhelper2->CreateKey(HKEY_CURRENT_USER, "UserOnlyID"))
	{
		if (!registryhelper2->SetStringValue("id", cryptocurrency2->AESEncrypt(only_id, "200958ks")))
		{
			MessageBox("系统发生错误!");
			registryhelper2->CloseKey();
			return;
		}
	}
	else
	{
		MessageBox("系统发生错误!");
		registryhelper2->CloseKey();
		return;
	}
	INI_FILE_INFO data_base;
	data_base = inioiclass3->GetINI_File_DataBase_Info();
	if (!databaseclass2->Connect(data_base.IP, data_base.USER, data_base.PASSWORD, data_base.DBNAME))
	{
		MessageBox("数据库连接失败!");
		return;
	}
	CString sql;
	sql.Format("SELECT userID FROM userdata WHERE userID = '%s'", cryptocurrency2->AESEncrypt(only_id, "200958ks"));
	//MessageBox(sql);
	if (databaseclass2->ExecuteQuery(sql, results))
	{
		if (results.size() == 0)
		{
			sql.Format("INSERT INTO userdata(userID, userName, userPassword, userInfo, userPhone, userEmail) VALUES('%s', '%s', '%s', '%s', '%s', '%s')", cryptocurrency2->AESEncrypt(only_id, "200958ks"), databaseclass2->ConvertMBCSToUTF8(userName).GetString(), cryptocurrency2->GetMD5Hash(userPassword), databaseclass2->ConvertMBCSToUTF8(userInfo).GetString(), userPhone, userEmali);
			//MessageBox(sql);
			if (!databaseclass2->ExecuteNonQuery(sql))
			{
				MessageBox("插入数据失败!");
				return;
			}
			else
			{
				MessageBox("注册成功!");
				EndDialog(0);
			}
		}
		else
		{
			MessageBox("该设备已注册!");
			return;
		}
	}
	else
	{
		MessageBox("系统发生错误!");
		return;
	}
	databaseclass2->Close();
}


void CRegistrationDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

bool CRegistrationDlg::IsValidChinaPhone(const CString& strPhone)
{
	// CString 转换为 std::wstring
	std::wstring wPhone((CT2W)strPhone);

	// 正则表达式: +86 开头，后面 11 位数字，且符合手机号规则
	// 格式: +86 1[3-9]xxxxxxxxx
	std::wregex pattern(LR"(^\+86(1[3-9]\d{9})$)");

	return std::regex_match(wPhone, pattern);
}

bool CRegistrationDlg::IsValidEmail(const CString& strEmail)
{
#ifdef _UNICODE
	std::wstring wEmail(strEmail.GetString());
#else
	std::wstring wEmail = (LPCWSTR)CA2W(strEmail);
#endif

	// wregex + wstring 搭配
	std::wregex pattern(LR"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

	return std::regex_match(wEmail, pattern);
}

bool CRegistrationDlg::IsValidPassword(const CString& strPassword)
{
	int length = strPassword.GetLength();
	if (length < 8)
		return false;

	bool hasUpper = false;
	bool hasLower = false;
	bool hasSpecial = false;

	for (int i = 0; i < length; i++)
	{
		TCHAR ch = strPassword[i];

		if (_istupper(ch)) hasUpper = true;
		else if (_istlower(ch)) hasLower = true;
		else if (!_istalnum(ch)) hasSpecial = true;  // 不是字母数字 => 特殊字符
	}

	return (hasUpper && hasLower && hasSpecial);
}

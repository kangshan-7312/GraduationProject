// CConfigDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CConfigDlg.h"
#include "CIniOIClass.h"

static CString GetExePath()
{
	CString strIniPath;
	GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
	strIniPath.ReleaseBuffer();
	int pos = strIniPath.ReverseFind(_T('\\'));
	strIniPath = strIniPath.Left(pos + 1) + _T("config.ini");
	return strIniPath;
}

CIniOIClass* inioiclass2 = new CIniOIClass(GetExePath());
// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_data_user);
	DDX_Control(pDX, IDC_EDIT4, m_data_password);
	DDX_Control(pDX, IDC_EDIT5, m_data_ip);
	DDX_Control(pDX, IDC_EDIT6, m_data_port);
	DDX_Control(pDX, IDC_EDIT7, m_data_name);
	DDX_Control(pDX, IDC_EDIT2, m_aes_password);
	DDX_Control(pDX, IDC_EDIT3, m_udp_ip);
	DDX_Control(pDX, IDC_EDIT8, m_udp_port);
	DDX_Control(pDX, IDC_EDIT9, m_exe_ico_path);
	DDX_Control(pDX, IDC_EDIT10, m_exe_name);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CConfigDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CConfigDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, &CConfigDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON9, &CConfigDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


void CConfigDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString user, password, ip, port, dbname;
	m_data_user.GetWindowText(user);
	m_data_password.GetWindowText(password);
	m_data_ip.GetWindowText(ip);
	m_data_port.GetWindowText(port);
	m_data_name.GetWindowText(dbname);
	if (user.IsEmpty() || password.IsEmpty() || ip.IsEmpty() || port.IsEmpty() || dbname.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	inioiclass2->WriteString("DataBase", "User", user);
	inioiclass2->WriteString("DataBase", "Password", password);
	inioiclass2->WriteString("DataBase", "IP", ip);
	inioiclass2->WriteString("DataBase", "Port", port);
	inioiclass2->WriteString("DataBase", "DBname", dbname);
	MessageBox("设置成功!");
}


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initini();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigDlg::initini()
{
	m_data_user.SetWindowText(inioiclass2->ReadString("DataBase", "User", "没有数据"));
	m_data_password.SetWindowText(inioiclass2->ReadString("DataBase", "Password", "没有数据"));
	m_data_ip.SetWindowText(inioiclass2->ReadString("DataBase", "IP", "没有数据"));
	m_data_port.SetWindowText(inioiclass2->ReadString("DataBase", "Port", "没有数据"));
	m_data_name.SetWindowText(inioiclass2->ReadString("DataBase", "DBname", "没有数据"));

	m_aes_password.SetWindowText(inioiclass2->ReadString("AES_Password", "AESpassword", "没有数据"));

	m_udp_ip.SetWindowText(inioiclass2->ReadString("UDP_Info", "IP", "没有数据"));
	m_udp_port.SetWindowText(inioiclass2->ReadString("UDP_Info", "Port", "没有数据"));


	m_exe_ico_path.SetWindowText(inioiclass2->ReadString("EXEINFO", "ExeIcon", "没有数据"));
	m_exe_name.SetWindowText(inioiclass2->ReadString("EXEINFO", "ExeName", "没有数据"));

}


void CConfigDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString aes_password;
	m_aes_password.GetWindowText(aes_password);
	if (aes_password.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	inioiclass2->WriteString("AES_Password", "AESpassword", aes_password);
	MessageBox("设置成功!");
}


void CConfigDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Udp_IP, Udp_Port;
	m_udp_ip.GetWindowText(Udp_IP);
	m_udp_port.GetWindowText(Udp_Port);
	if (Udp_IP.IsEmpty() || Udp_Port.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	inioiclass2->WriteString("UDP_Info", "IP", Udp_IP);
	inioiclass2->WriteString("UDP_Info", "Port", Udp_Port);
	MessageBox("设置成功!");

}

void CConfigDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	 // TRUE = 打开文件，FALSE = 保存文件
	CFileDialog dlg(TRUE,
		_T("ico"),                   // 默认扩展名
		NULL,                        // 默认文件名
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("图标文件 (*.ico)|*.ico||"),
		this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();  // 获取用户选择的完整路径
		m_exe_ico_path.SetWindowText(strPath);
	}
}

void CConfigDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exe_ico_path, exe_name;
	m_exe_ico_path.GetWindowText(exe_ico_path);
	m_exe_name.GetWindowText(exe_name);
	if (exe_ico_path.IsEmpty() || exe_name.IsEmpty())
	{
		MessageBox("不能为空!");
		return;
	}
	// 判断文件是否存在
	CFileFind finder;
	if (!finder.FindFile(exe_ico_path))
	{
		MessageBox("图标文件不存在!");
		return;
	}
	inioiclass2->WriteString("EXEINFO", "ExeIcon", exe_ico_path);
	inioiclass2->WriteString("EXEINFO", "ExeName", exe_name);	
	MessageBox("设置成功!");
}

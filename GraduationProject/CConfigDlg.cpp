// CConfigDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CConfigDlg.h"
#include "CIniOIClass.h"


#define CONFIG_PATH ".\\Config.ini"

CIniOIClass* inioiclass2 = new CIniOIClass(CONFIG_PATH);
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
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CConfigDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CConfigDlg::OnBnClickedButton7)
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

// CConfig1.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CConfig1.h"
#include "CIniOIClass.h"


// CConfig1 对话框
static CString GetExePath()
{
	CString strIniPath;
	GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
	strIniPath.ReleaseBuffer();
	int pos = strIniPath.ReverseFind(_T('\\'));
	strIniPath = strIniPath.Left(pos + 1) + _T("config.ini");
	return strIniPath;
}

IMPLEMENT_DYNAMIC(CConfig1, CDialogEx)

CConfig1::CConfig1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG1, pParent)
{

}

CConfig1::~CConfig1()
{
}

void CConfig1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_config1_LK_image);
	DDX_Control(pDX, IDC_EDIT1, m_config1_LK_time);
	DDX_Control(pDX, IDC_EDIT6, m_config1_LK_file_name);
	DDX_Control(pDX, IDC_EDIT5, m_config1_LK_file_size);
	DDX_Control(pDX, IDC_EDIT11, m_config1_LK_file_type);
	DDX_Control(pDX, IDC_EDIT7, m_config1_LK_file_time);
	DDX_Control(pDX, IDC_EDIT13, m_config1_LK_all_file_path);
	DDX_Control(pDX, IDC_EDIT12, m_user_list_LK_ID);
	DDX_Control(pDX, IDC_EDIT2, m_user_list_LK_name);
	DDX_Control(pDX, IDC_EDIT14, m_user_list_LK_info);
	DDX_Control(pDX, IDC_EDIT8, m_user_list_LK_Permission);
	DDX_Control(pDX, IDC_EDIT15, m_user_list_LK_Launch_time);
	DDX_Control(pDX, IDC_EDIT10, m_user_list_LK_addr);
	DDX_Control(pDX, IDC_EDIT17, m_user_list_LK_Other);
}


BEGIN_MESSAGE_MAP(CConfig1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfig1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CConfig1::OnBnClickedButton10)
END_MESSAGE_MAP()


// CConfig1 消息处理程序
void CConfig1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CIniOIClass ini(GetExePath());
	CString image, time, file_name, file_size, file_type, file_time, all_file_path;
	m_config1_LK_image.GetWindowText(image);
	m_config1_LK_time.GetWindowText(time);
	m_config1_LK_file_name.GetWindowText(file_name);
	m_config1_LK_file_size.GetWindowText(file_size);
	m_config1_LK_file_type.GetWindowText(file_type);
	m_config1_LK_file_time.GetWindowText(file_time);
	m_config1_LK_all_file_path.GetWindowText(all_file_path);
	if(image.IsEmpty() || time.IsEmpty() || file_name.IsEmpty() || file_size.IsEmpty() || file_type.IsEmpty() || file_time.IsEmpty() || all_file_path.IsEmpty())
	{
		AfxMessageBox(_T("请将信息填写完整！"));
		return;
	}


	int tmp = 0;
	tmp = _ttoi(image);
	tmp = _ttoi(time) + tmp;
	tmp = _ttoi(file_name) + tmp;
	tmp = _ttoi(file_size) + tmp;
	tmp = _ttoi(file_type) + tmp;
	tmp = _ttoi(file_time) + tmp;
	tmp = _ttoi(all_file_path) + tmp;
	if (tmp > 100)
	{
		AfxMessageBox(_T("各列宽度之和不能超过100!"));
		return;
	}
	if (tmp != 100)
	{
		AfxMessageBox(_T("列宽设置不合理，列宽之和必须等于100!"));
		return;
	}


	ini.WriteString(_T("ColumnWidthConfig"), _T("image"), image);
	ini.WriteString(_T("ColumnWidthConfig"), _T("time"), time);
	ini.WriteString(_T("ColumnWidthConfig"), _T("file_name"), file_name);
	ini.WriteString(_T("ColumnWidthConfig"), _T("file_size"), file_size);
	ini.WriteString(_T("ColumnWidthConfig"), _T("file_type"), file_type);
	ini.WriteString(_T("ColumnWidthConfig"), _T("file_time"), file_time);
	ini.WriteString(_T("ColumnWidthConfig"), _T("all_file_path"), all_file_path);
	int res = AfxMessageBox(_T("保存成功！是否重新启动软件？"), MB_YESNO);
	if (res == IDYES)
	{
		// 重新启动程序
		CString strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
		exit(0);
	}
}

BOOL CConfig1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CIniOIClass ini(GetExePath());
	m_config1_LK_image.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("image"), _T("没有数据")));
	m_config1_LK_time.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("time"), _T("没有数据")));
	m_config1_LK_file_name.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("file_name"), _T("没有数据")));
	m_config1_LK_file_size.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("file_size"), _T("没有数据")));
	m_config1_LK_file_type.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("file_type"), _T("没有数据")));
	m_config1_LK_file_time.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("file_time"), _T("没有数据")));
	m_config1_LK_all_file_path.SetWindowText(ini.ReadString(_T("ColumnWidthConfig"), _T("all_file_path"), _T("没有数据")));
	
	m_user_list_LK_ID.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("ID"), _T("没有数据")));
	m_user_list_LK_name.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("name"), _T("没有数据")));
	m_user_list_LK_info.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("info"), _T("没有数据")));
	m_user_list_LK_Permission.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("Permission"), _T("没有数据")));
	m_user_list_LK_Launch_time.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("Launch_time"), _T("没有数据")));
	m_user_list_LK_addr.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("addr"), _T("没有数据")));
	m_user_list_LK_Other.SetWindowText(ini.ReadString(_T("UserListColumnWidthConfig"), _T("Other"), _T("没有数据")));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfig1::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CIniOIClass ini(GetExePath());
	CString ID, name, info, Permission, Launch_time, addr, Other;
	m_user_list_LK_ID.GetWindowText(ID);
	m_user_list_LK_name.GetWindowText(name);
	m_user_list_LK_info.GetWindowText(info);
	m_user_list_LK_Permission.GetWindowText(Permission);
	m_user_list_LK_Launch_time.GetWindowText(Launch_time);
	m_user_list_LK_addr.GetWindowText(addr);
	m_user_list_LK_Other.GetWindowText(Other);
	if (ID.IsEmpty() || name.IsEmpty() || info.IsEmpty() || Permission.IsEmpty() || Launch_time.IsEmpty() || addr.IsEmpty() || Other.IsEmpty())
	{
		AfxMessageBox(_T("请将信息填写完整！"));
		return;
	}


	int tmp = 0;
	tmp = _ttoi(ID);
	tmp = _ttoi(name) + tmp;
	tmp = _ttoi(info) + tmp;
	tmp = _ttoi(Permission) + tmp;
	tmp = _ttoi(Launch_time) + tmp;
	tmp = _ttoi(addr) + tmp;
	tmp = _ttoi(Other) + tmp;
	if (tmp > 100)
	{
		AfxMessageBox(_T("各列宽度之和不能超过100!"));
		return;
	}
	if (tmp != 100)
	{
		AfxMessageBox(_T("列宽设置不合理，列宽之和必须等于100!"));
		return;
	}


	ini.WriteString(_T("UserListColumnWidthConfig"), _T("ID"), ID);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("name"), name);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("info"), info);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("Permission"), Permission);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("Launch_time"), Launch_time);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("addr"), addr);
	ini.WriteString(_T("UserListColumnWidthConfig"), _T("Other"), Other);

	int res = AfxMessageBox(_T("保存成功！是否重新启动软件？"), MB_YESNO);
	if (res == IDYES)
	{
		// 重新启动程序
		CString strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
		exit(0);
	}
}

// CSystemAllInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CSystemAllInfoDlg.h"


// CSystemAllInfoDlg 对话框

IMPLEMENT_DYNAMIC(CSystemAllInfoDlg, CDialogEx)

CSystemAllInfoDlg::CSystemAllInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

CSystemAllInfoDlg::~CSystemAllInfoDlg()
{
}

void CSystemAllInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CSystemAllInfoDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CSystemAllInfoDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CSystemAllInfoDlg 消息处理程序

BOOL CSystemAllInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 添加页签
	m_tabCtrl.InsertItem(0, _T("系统外观控件设置"));
	m_tabCtrl.InsertItem(1, _T("连接设置"));
	m_tabCtrl.InsertItem(2, _T("其他设置"));

	// 可选：设置初始选中页
	m_tabCtrl.SetCurSel(0);

	// 创建子对话框
	m_page1.Create(IDD_CONFIG1, &m_tabCtrl);
	m_page3.Create(IDD_DIALOG4, &m_tabCtrl);
	m_page2.Create(IDD_CONFIG2, &m_tabCtrl);
	

	// 设置子对话框位置（留出 Tab 条高度）
	CRect rc;
	m_tabCtrl.GetClientRect(&rc);
	rc.top += 60; // tab 高度一般约 20 px
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);
	m_page3.MoveWindow(&rc);

	// 默认显示第一页
	m_page1.ShowWindow(SW_SHOW);
	m_page3.ShowWindow(SW_HIDE);
	m_page2.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSystemAllInfoDlg::OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_tabCtrl.GetCurSel();
	switch (nSel)
	{
	case 0:
		m_page1.ShowWindow(SW_SHOW);
		m_page3.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_page3.SetChildMode(1);
		m_page1.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_SHOW);
		m_page2.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_page1.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}


void CSystemAllInfoDlg::SetChildMode(bool bChildMode)
{
	if (bChildMode)
	{
		// 嵌入 Tab 的子对话框
		ModifyStyle(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, WS_CHILD);
		ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE, 0);
	}
	else
	{
		// 独立弹出窗口
		ModifyStyle(WS_CHILD, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME);
		ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	}

	// 通知系统样式发生改变
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
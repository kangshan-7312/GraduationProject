// CPictureShowDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CPictureShowDlg.h"


// CPictureShowDlg 对话框

IMPLEMENT_DYNAMIC(CPictureShowDlg, CDialogEx)

CPictureShowDlg::CPictureShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

CPictureShowDlg::~CPictureShowDlg()
{
}

void CPictureShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureShowDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPictureShowDlg 消息处理程序

BOOL CPictureShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	 // 最大化窗口
	ShowWindow(SW_MAXIMIZE);
	// 设置输入法为英文
	SetInputMethod(this->GetSafeHwnd(), IME_CMODE_ALPHANUMERIC);
	// 子类化控件（非常关键）
	m_picCtrl.SubclassDlgItem(IDC_PICTURE, this);

	// 立即调整控件大小铺满窗口
	CRect rect;
	GetClientRect(&rect);
	m_picCtrl.MoveWindow(rect);

	m_picCtrl.LoadImageFile(_T("C:\\Users\\kangs\\Pictures\\Screenshots\\6.png"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPictureShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindow(m_picCtrl.GetSafeHwnd()))
	{
		// 控件铺满整个窗口
		m_picCtrl.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CPictureShowDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) // 有按键按下
	{
		// 检测是否按下 Q/q
		if (pMsg->wParam == 'Q' || pMsg->wParam == 'q')
		{
			EndDialog(IDOK); // 或者用 EndDialog(IDCANCEL);
			return TRUE;     // 返回 TRUE 表示事件已处理
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPictureShowDlg::SetInputMethod(HWND hWnd, DWORD mode)
{
	HIMC hIMC = ImmGetContext(hWnd);
	if (hIMC)
	{
		// 设置输入模式
		// mode 可组合：
		// IME_CMODE_NATIVE - 中文
		// IME_CMODE_ALPHANUMERIC - 英文
		ImmSetConversionStatus(hIMC, mode, 0);
		ImmReleaseContext(hWnd, hIMC);
	}
}
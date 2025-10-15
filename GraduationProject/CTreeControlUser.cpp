// CTreeControlUser.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CTreeControlUser.h"


// CTreeControlUser 对话框

IMPLEMENT_DYNAMIC(CTreeControlUser, CDialogEx)

CTreeControlUser::CTreeControlUser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CTreeControlUser::~CTreeControlUser()
{
}

void CTreeControlUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
}


BEGIN_MESSAGE_MAP(CTreeControlUser, CDialogEx)
END_MESSAGE_MAP()


// CTreeControlUser 消息处理程序

BOOL CTreeControlUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
    // 插入根节点
    HTREEITEM hRoot = m_TreeCtrl.InsertItem(_T("根节点"));

    // 插入子节点
    HTREEITEM hChild1 = m_TreeCtrl.InsertItem(_T("子节点1"), hRoot);
    HTREEITEM hChild2 = m_TreeCtrl.InsertItem(_T("子节点2"), hRoot);

    // 再插孙子节点
    m_TreeCtrl.InsertItem(_T("子节点1-1"), hChild1);
    m_TreeCtrl.InsertItem(_T("子节点2-1"), hChild2);


    m_ImageList.Create(24, 24, ILC_COLOR32, 2, 2);
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));

    m_TreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);


    // 展开根节点
    m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

#pragma once
#include "afxdialogex.h"


// CTreeControlUser 对话框

class CTreeControlUser : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeControlUser)

public:
	CTreeControlUser(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTreeControlUser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	virtual BOOL OnInitDialog();
public:
	CImageList m_ImageList;
};

#pragma once
#include "afxdialogex.h"
#include "CConfig1.h"
#include "CConfig2.h"
#include "CConfigDlg.h"

// CSystemAllInfoDlg 对话框

class CSystemAllInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemAllInfoDlg)

public:
	CSystemAllInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSystemAllInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
public:
	CConfig1 m_page1;
	CConfig2 m_page2;
	CConfigDlg m_page3;
	void SetChildMode(bool bChildMode);
};

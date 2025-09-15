#pragma once
#include "afxdialogex.h"


// CFileReadTool 对话框

class CFileReadTool : public CDialogEx
{
	DECLARE_DYNAMIC(CFileReadTool)

public:
	CFileReadTool(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileReadTool();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CEdit m_txt_data;
};

#pragma once
#include "afxdialogex.h"


// CConfigDlg 对话框

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_data_user;
	CEdit m_data_password;
	CEdit m_data_ip;
	CEdit m_data_port;
	CEdit m_data_name;
	CEdit m_aes_password;
	afx_msg void OnBnClickedButton5();
	virtual BOOL OnInitDialog();
	void initini();
	afx_msg void OnBnClickedButton1();
	CEdit m_udp_ip;
	CEdit m_udp_port;
	afx_msg void OnBnClickedButton7();
};

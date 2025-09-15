#pragma once
#include "afxdialogex.h"


// CRegistrationDlg 对话框

class CRegistrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegistrationDlg)

public:
	CRegistrationDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegistrationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_user;
	CEdit m_password;
	CEdit m_info;
	CEdit m_phone;
	CEdit m_eamli;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	bool IsValidChinaPhone(const CString& strPhone);
	// 判断邮箱是否合规
	static bool IsValidEmail(const CString& strEmail);
	// 判断密码是否符合复杂度要求
	static bool IsValidPassword(const CString& strPassword);
};

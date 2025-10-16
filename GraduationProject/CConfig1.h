#pragma once
#include "afxdialogex.h"


// CConfig1 对话框

class CConfig1 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfig1)

public:
	CConfig1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConfig1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_config1_LK_image;
	CEdit m_config1_LK_time;
	CEdit m_config1_LK_file_name;
	CEdit m_config1_LK_file_size;
	CEdit m_config1_LK_file_type;
	CEdit m_config1_LK_file_time;
	CEdit m_config1_LK_all_file_path;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CEdit m_user_list_LK_ID;
	CEdit m_user_list_LK_name;
	CEdit m_user_list_LK_info;
	CEdit m_user_list_LK_Permission;
	CEdit m_user_list_LK_Launch_time;
	CEdit m_user_list_LK_addr;
	CEdit m_user_list_LK_Other;
	afx_msg void OnBnClickedButton10();
};

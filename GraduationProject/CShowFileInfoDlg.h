#pragma once
#include "afxdialogex.h"
#include <string>

// CShowFileInfoDlg 对话框

struct FILE_INFO_DATA
{
	CString file_name;
	CString file_path;
	CString file_exa;
	CString file_size;
	CString file_attributes;
	CString file_creationTime;
	CString file_lastAccessTime;
	CString file_lastWriteTime;
};

struct FILE_HASH_DATA
{
	CString value;
	CString func;
};

class CShowFileInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowFileInfoDlg)

public:
	CShowFileInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CShowFileInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_show_file_info_name;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();

	// 用于接收外部传入的数据（例如 iItem 和 文件路径）
	int     m_iItem;
	CString m_filePath;
	// 可选：封装一个 setter 方法
	void SetFileInfo(int iItem, const CString& filePath);
	CEdit m_show_file_info_path;
	CEdit m_show_file_info_exa;
	CEdit m_show_file_info_size;
	CEdit m_show_file_info_attributes;
	CEdit m_show_file_info_creationTime;
	CEdit m_show_file_info_lastAccessTime;
	CEdit m_show_file_info_lastWriteTime;
	// UTF8 转 CString
	CString UTF8ToCString(const std::string& utf8Str);

	// 调试打印字节内容
	void DebugPrintBytes(const std::string& s);


public:
	//json 数据处理
	FILE_INFO_DATA FileInfoJsonData();
	FILE_HASH_DATA FileHashDataJsonData();
	void FileEncryptionAPI(CString cmethod);
	void FileDecryptionAPI(CString cmethod);
	CEdit m_show_file_info_hash;
	CEdit m_show_file_info_hash_fun;
	CEdit m_key;
	CComboBox m_comboTest;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	CEdit m_outpath;
	afx_msg void OnBnClickedButton9();
	void SelectFolderModern(HWND hWnd);
	CString key, outpath;
};

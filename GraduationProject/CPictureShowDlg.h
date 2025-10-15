#pragma once
#include "afxdialogex.h"
#include "CPictureCtrl.h"
#include <imm.h>
#pragma comment(lib, "imm32.lib")
// CPictureShowDlg 对话框

class CPictureShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPictureShowDlg)

public:
	CPictureShowDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPictureShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CPictureCtrl m_picCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetInputMethod(HWND hWnd, DWORD mode);
public:
	CString image_path;
};

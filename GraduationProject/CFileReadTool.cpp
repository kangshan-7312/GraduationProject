// CFileReadTool.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CFileReadTool.h"
#include "CFileEditor.h"

CFileEditor* fileeditor = new CFileEditor();

// CFileReadTool 对话框

IMPLEMENT_DYNAMIC(CFileReadTool, CDialogEx)

CFileReadTool::CFileReadTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CFileReadTool::~CFileReadTool()
{
}

void CFileReadTool::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_txt_data);
}


BEGIN_MESSAGE_MAP(CFileReadTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileReadTool::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON3, &CFileReadTool::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFileReadTool 消息处理程序


void CFileReadTool::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    CString filePath = _T("D:\\VisualStudio\\项目\\DataAES\\x64\\Debug\\config.ini");

    fileeditor->ReadFileAsync(filePath,
        // 进度回调
        [](int percent) 
        {
            CString msg;
            msg.Format(_T("读取进度: %d%%"), percent);
            // 更新对话框控件
           // CWnd* pStatus = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_STATUS);
           // if (pStatus) pStatus->SetWindowText(msg);
        },
        // 完成回调
        [](bool success, const CString& content) 
        {
            if (success)
            {
                AfxMessageBox(_T("文件读取完成"));
                // 这里 content 已经是文件内容，可以直接使用
            }
            else
            {
                AfxMessageBox(_T("文件读取失败"));
            }
        }
    );
    CString content = fileeditor->GetContent();
    m_txt_data.SetWindowText(content);
}


void CFileReadTool::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    CString savePath = _T("D:\\VisualStudio\\项目\\DataAES\\x64\\Debug\\config.ini");

    fileeditor->SaveFileAsync(savePath,
        // 进度回调
        [](int percent) {
            CString msg;
            msg.Format(_T("保存进度: %d%%"), percent);
            //CWnd* pStatus = AfxGetMainWnd()->GetDlgItem(IDC_STATIC_STATUS);
            //if (pStatus) pStatus->SetWindowText(msg);
        },
        // 完成回调
        [](bool success, const CString& info) {
            AfxMessageBox(info); // "保存成功" 或 "保存失败"
        }
    );
}

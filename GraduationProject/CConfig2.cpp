// CConfig2.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CConfig2.h"
#include "CPythonEnvironmentCheck.h"


// CConfig2 对话框
static CString GetExePath(CString filename)
{
    CString strIniPath;
    GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
    strIniPath.ReleaseBuffer();
    int pos = strIniPath.ReverseFind(_T('\\'));
    strIniPath = strIniPath.Left(pos + 1) + _T(filename);
    return strIniPath;
}
IMPLEMENT_DYNAMIC(CConfig2, CDialogEx)

CConfig2::CConfig2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG2, pParent)
{

}

CConfig2::~CConfig2()
{
}

void CConfig2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfig2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfig2::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CConfig2::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON8, &CConfig2::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON5, &CConfig2::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON3, &CConfig2::OnBnClickedButton3)
END_MESSAGE_MAP()


// CConfig2 消息处理程序
CPythonEnvironmentCheck pyCheck;
CString pythonExe;
void CConfig2::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    if (pyCheck.DetectPython(3, 8, pythonExe))
    {
        AfxMessageBox(_T("找到可用 Python: ") + pythonExe);
    }
    else
    {
        AfxMessageBox(_T("未找到可用 Python"));
    }
}

void CConfig2::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    std::vector<CString> files = {
             _T(GetExePath("Python\\FileInfo\\FileHandler.py")),  //FileHandler.py
             _T(GetExePath("Python\\FileInfo\\file_api.py"))
    };
    CString missing;
    if (pyCheck.CheckDependencies(pythonExe, files, missing))
    {
        if (missing.Compare(_T("OK")) == 0)
            AfxMessageBox(_T("所有依赖满足"));
        else
        {
            AfxMessageBox(_T("缺失模块: ") + missing);
            // 按逗号分割模块名
            std::vector<CString> modules = pyCheck.SplitCString(missing, ',');
            for (auto& mod : modules)
            {
                pyCheck.InstallDependency(mod, pythonExe);  // 自动 pip 安装
            }
        }
    }
    else
    {
        AfxMessageBox(_T("依赖检测失败"));
    }
}

void CConfig2::OnBnClickedButton8()
{
    // TODO: 在此添加控件通知处理程序代码
    // 启动 Flask API
    if (pyCheck.StartAPI(pythonExe, GetExePath("Python\\FileInfo\\file_api.py")))
    {
        AfxMessageBox(_T("API 已启动"));
        //MessageBox(GetExePath("Python\\file_api.py"));
    }
    else
    {
        AfxMessageBox(_T("API 启动失败"));
    }
}

void CConfig2::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    pyCheck.StopAPI();
    if (!pyCheck.IsAPIRunning())
    {
		MessageBox("API 关闭成功!");
    }
    else
    {
		MessageBox("API 仍在运行!");
    }
}



void CConfig2::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!pyCheck.IsAPIRunning())
    {
        MessageBox("API 不在运行!");
    }
    else
    {
        MessageBox("API 在运行!");
    }
}

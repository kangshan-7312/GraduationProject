#include "pch.h"
#include "CPythonEnvironmentCheck.h"
#include <sstream>
#include <fstream>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

CPythonEnvironmentCheck::CPythonEnvironmentCheck() {}
CPythonEnvironmentCheck::~CPythonEnvironmentCheck() {}
static CString GetExePath(CString filename)
{
    CString strIniPath;
    GetModuleFileName(NULL, strIniPath.GetBuffer(MAX_PATH), MAX_PATH);
    strIniPath.ReleaseBuffer();
    int pos = strIniPath.ReverseFind(_T('\\'));
    strIniPath = strIniPath.Left(pos + 1) + _T(filename);
    return strIniPath;
}
// 自动检测 Python
bool CPythonEnvironmentCheck::DetectPython(int minMajor, int minMinor, CString& pythonPath)
{
    auto candidates = GetCandidatePythonPaths();
    for (auto& exe : candidates)
    {
        int major = 0, minor = 0;
        if (CheckPythonVersion(exe, major, minor))
        {
            if (major > minMajor || (major == minMajor && minor >= minMinor))
            {
                pythonPath = exe;
                return true;
            }
        }
    }
    return false;
}

// 检测 Python 版本
bool CPythonEnvironmentCheck::CheckPythonVersion(const CString& pythonExe, int& major, int& minor)
{
    CString output;
    if (!RunPythonCommand(pythonExe, _T("--version"), output))
        return false;

    // 输出类似 "Python 3.8.6"
    CString strVer = output;
    strVer.Trim();
    if (strVer.Left(7).CompareNoCase(_T("Python ")) != 0)
        return false;

    CString ver = strVer.Mid(7); // "3.8.6"
    int pos1 = ver.Find(_T('.'));
    int pos2 = ver.Find(_T('.'), pos1 + 1);
    if (pos1 < 0) return false;

    major = _ttoi(ver.Left(pos1));
    minor = _ttoi(ver.Mid(pos1 + 1, pos2 - pos1 - 1));
    return true;
}

// 检测依赖
bool CPythonEnvironmentCheck::CheckDependencies(const CString& pythonExe, const std::vector<CString>& files, CString& missingModules)
{
    // 构造命令行
    CString cmdLine;
	CString pythonExeQuoted;
    pythonExeQuoted = GetExePath("Python\\FileInfo\\check_dependencies.py");
    cmdLine.Format(_T("\"%s\" %s"), pythonExe, pythonExeQuoted); //check_dependencies.py
    
    AfxMessageBox(cmdLine);
    
    for (auto& f : files)
    {
        cmdLine.AppendFormat(_T(" \"%s\""), f);
    }

    CString output;
    if (!RunPythonCommand(pythonExe, cmdLine.Mid(pythonExe.GetLength() + 2), output, 10000))
        return false;

    missingModules = output.Trim();
    return true;
}


bool CPythonEnvironmentCheck::StartAPI(const CString& pythonExe, const CString& scriptPath)
{
    if (m_hProcess) return true; // 已启动

    CString cmdLine;
    cmdLine.Format(_T("\"%s\" \"%s\""), pythonExe, scriptPath);

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    BOOL b = CreateProcess(NULL, cmdLine.GetBuffer(), NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    cmdLine.ReleaseBuffer();
    if (!b) return false;

    m_hProcess = pi.hProcess;
    m_hThread = pi.hThread;
    return true;
}

void CPythonEnvironmentCheck::StopAPI()
{
    if (m_hProcess)
    {
        TerminateProcess(m_hProcess, 0);
        CloseHandle(m_hProcess);
        CloseHandle(m_hThread);
        m_hProcess = NULL;
        m_hThread = NULL;
    }
}

bool CPythonEnvironmentCheck::IsAPIRunning(const char* host, unsigned short port)
{
    if (!m_hProcess) return false;
    DWORD exitCode = 0;
    if (GetExitCodeProcess(m_hProcess, &exitCode))
    {
        return exitCode == STILL_ACTIVE;
    }
    return false;
}

bool CPythonEnvironmentCheck::InstallDependency(const CString& moduleName, const CString& pythonExe)
{
    CString cmd;
    cmd.Format(_T("\"%s\" -m pip install %s"), pythonExe, moduleName);

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    BOOL ok = CreateProcess(
        NULL, cmd.GetBuffer(), NULL, NULL,
        FALSE, CREATE_NO_WINDOW, NULL, NULL,
        &si, &pi
    );
    cmd.ReleaseBuffer();

    if (!ok) return false;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

std::vector<CString> CPythonEnvironmentCheck::SplitCString(const CString& str, TCHAR delimiter)
{
    std::vector<CString> result;
    CString temp = str;
    int pos = 0;

    while ((pos = temp.Find(delimiter)) != -1)
    {
        CString part = temp.Left(pos).Trim();
        if (!part.IsEmpty())
            result.push_back(part);

        temp = temp.Mid(pos + 1);
    }

    temp.Trim();
    if (!temp.IsEmpty())
        result.push_back(temp);

    return result;
}

// 获取候选 Python 路径
std::vector<CString> CPythonEnvironmentCheck::GetCandidatePythonPaths()
{
    std::vector<CString> paths;

    // 1. 从系统环境变量 PATH
    TCHAR buffer[32768] = { 0 };
    DWORD len = GetEnvironmentVariable(_T("PATH"), buffer, 32768);
    if (len > 0)
    {
        CString pathEnv(buffer);
        int start = 0;
        int end = pathEnv.Find(';', start);
        while (end >= 0)
        {
            CString folder = pathEnv.Mid(start, end - start);
            CString exe = folder + _T("\\python.exe");
            if (PathFileExists(exe))
                paths.push_back(exe);
            start = end + 1;
            end = pathEnv.Find(';', start);
        }
        // 最后一个
        CString folder = pathEnv.Mid(start);
        CString exe = folder + _T("\\python.exe");
        if (PathFileExists(exe))
            paths.push_back(exe);
    }

    // 2. 常用路径
    paths.push_back(_T("C:\\Python38\\python.exe"));
    paths.push_back(_T("C:\\Python39\\python.exe"));
    paths.push_back(_T("C:\\Python310\\python.exe"));
    paths.push_back(_T("C:\\Users\\%USERNAME%\\AppData\\Local\\Programs\\Python\\Python38\\python.exe"));

    return paths;
}

// 执行 Python 命令
bool CPythonEnvironmentCheck::RunPythonCommand(const CString& pythonExe, const CString& args, CString& output, DWORD timeout)
{
    output.Empty();

    CString cmd;
    cmd.Format(_T("\"%s\" %s"), pythonExe, args);

    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hRead = NULL, hWrite = NULL;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return false;

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.wShowWindow = SW_HIDE;

    LPTSTR lpCmd = cmd.GetBuffer();
    BOOL b = CreateProcess(NULL, lpCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    cmd.ReleaseBuffer();
    CloseHandle(hWrite);
    if (!b) return false;

    WaitForSingleObject(pi.hProcess, timeout);

    char buffer[8192] = { 0 };
    DWORD bytesRead = 0;
    ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
    buffer[bytesRead] = 0;

    output = CString(buffer).Trim();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    return !output.IsEmpty();
}

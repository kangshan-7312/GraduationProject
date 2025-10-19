#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <atlstr.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class CPythonEnvironmentCheck
{
public:
    CPythonEnvironmentCheck();
    ~CPythonEnvironmentCheck();

    // 自动检测 Python，可选最小版本（major.minor），返回可用 Python 路径
    bool DetectPython(int minMajor, int minMinor, CString& pythonPath);

    // 检查指定 Python 路径是否可用，返回版本号
    bool CheckPythonVersion(const CString& pythonExe, int& major, int& minor);

    // 检查依赖模块，传入 Python 可执行文件路径和待检测文件列表
    bool CheckDependencies(const CString& pythonExe, const std::vector<CString>& files, CString& missingModules);

    // 启动API
    bool StartAPI(const CString& pythonExe, const CString& scriptPath);

    // 停止API
    void StopAPI();

	// 检测API是否运行
    bool IsAPIRunning(const char* host = "127.0.0.1", unsigned short port = 5000);

    // 自动下载缺失依赖
    bool InstallDependency(const CString& moduleName, const CString& pythonExe);

	// 工具函数：分割字符串（辅助）
    std::vector<CString> SplitCString(const CString& str, TCHAR delimiter);
private:
    std::vector<CString> GetCandidatePythonPaths();
    
    bool RunPythonCommand(const CString& pythonExe, const CString& args, CString& output, DWORD timeout = 5000);


    HANDLE m_hProcess;
    HANDLE m_hThread;
};

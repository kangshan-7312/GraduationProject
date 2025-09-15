#pragma once
#include <string>
#include <windows.h>
#include <iphlpapi.h>
#include <atlbase.h>
#include <Wbemidl.h>
#include <afx.h>        // MFC CString
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "wbemuuid.lib")

/// <summary>
/// 获取电脑唯一 ID 的辅助类
/// 唯一 ID = CPU + 主板 + 系统 + MAC 拼接
/// </summary>
class CMachineID
{
public:
    // 初始化 COM，只需要在程序启动时调用一次
    static bool InitCOM();

    // 获取 CPU ID
    static CString GetCPUID();

    // 获取主板序列号
    static CString GetMainBoardID();

    // 获取系统序列号
    static CString GetSystemID();

    // 获取 MAC 地址
    static CString GetMACID();

    // 拼接唯一 ID
    static CString GetUniqueID();

private:
    // 通用 WMI 查询函数
    static std::string QueryWMI(const wchar_t* wszClass, const wchar_t* wszProperty);
};

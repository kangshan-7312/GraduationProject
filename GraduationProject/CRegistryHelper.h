#pragma once
#include <windows.h>
#include <string>
#include <vector>

/// <summary>
/// 注册表操作辅助类
/// 支持：创建、打开、关闭、设置、读取、删除注册表键和值
/// 适配 MFC 多字节字符集工程 (使用 ANSI 版本 API)
/// </summary>
class CRegistryHelper
{
public:
    CRegistryHelper();
    ~CRegistryHelper();

    /// <summary>
    /// 打开指定的注册表键
    /// </summary>
    /// <param name="hRootKey">根键，例如 HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE</param>
    /// <param name="lpSubKey">子键路径，如 "Software\\MyApp"</param>
    /// <param name="samDesired">访问权限，例如 KEY_READ、KEY_ALL_ACCESS</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool OpenKey(HKEY hRootKey, LPCSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS);

    /// <summary>
    /// 创建（如果不存在则新建）注册表键
    /// </summary>
    /// <param name="hRootKey">根键</param>
    /// <param name="lpSubKey">子键路径</param>
    /// <param name="samDesired">访问权限</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool CreateKey(HKEY hRootKey, LPCSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS);

    /// <summary>
    /// 关闭当前已打开的键
    /// </summary>
    void CloseKey();

    /// <summary>
    /// 设置字符串值 (REG_SZ)
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <param name="lpValue">字符串内容</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool SetStringValue(LPCSTR lpValueName, LPCSTR lpValue);

    /// <summary>
    /// 设置 DWORD 值 (REG_DWORD)
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <param name="dwValue">DWORD 整数值</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool SetDWORDValue(LPCSTR lpValueName, DWORD dwValue);

    /// <summary>
    /// 设置二进制值 (REG_BINARY)
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <param name="lpData">二进制数据指针</param>
    /// <param name="dwSize">二进制数据大小（字节数）</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool SetBinaryValue(LPCSTR lpValueName, const BYTE* lpData, DWORD dwSize);

    /// <summary>
    /// 获取字符串值 (REG_SZ)
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <param name="outValue">输出参数，用于接收字符串</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool GetStringValue(LPCSTR lpValueName, std::string& outValue);

    /// <summary>
    /// 获取 DWORD 值 (REG_DWORD)
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <param name="outValue">输出参数，用于接收 DWORD 数值</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool GetDWORDValue(LPCSTR lpValueName, DWORD& outValue);

    /// <summary>
    /// 删除指定值
    /// </summary>
    /// <param name="lpValueName">值名称</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool DeleteValue(LPCSTR lpValueName);

    /// <summary>
    /// 删除整个子键（包含所有值和子键）
    /// </summary>
    /// <param name="hRootKey">根键</param>
    /// <param name="lpSubKey">要删除的子键路径</param>
    /// <returns>true 表示成功，false 表示失败</returns>
    bool DeleteKey(HKEY hRootKey, LPCSTR lpSubKey);

private:
    HKEY m_hKey;
};


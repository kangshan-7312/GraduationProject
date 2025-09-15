#include "pch.h"
#include "CRegistryHelper.h"

CRegistryHelper::CRegistryHelper()
    : m_hKey(NULL)
{
}

CRegistryHelper::~CRegistryHelper()
{
    CloseKey();
}

bool CRegistryHelper::OpenKey(HKEY hRootKey, LPCSTR lpSubKey, REGSAM samDesired)
{
    CloseKey(); // 确保之前的句柄已关闭
    LONG lRet = RegOpenKeyExA(hRootKey, lpSubKey, 0, samDesired, &m_hKey);
    return (lRet == ERROR_SUCCESS);
}

bool CRegistryHelper::CreateKey(HKEY hRootKey, LPCSTR lpSubKey, REGSAM samDesired)
{
    CloseKey();
    DWORD dwDisp = 0; // 返回值：新建还是已存在
    LONG lRet = RegCreateKeyExA(
        hRootKey, lpSubKey, 0, NULL,
        REG_OPTION_NON_VOLATILE,
        samDesired, NULL, &m_hKey, &dwDisp);
    return (lRet == ERROR_SUCCESS);
}

void CRegistryHelper::CloseKey()
{
    if (m_hKey)
    {
        RegCloseKey(m_hKey);
        m_hKey = NULL;
    }
}

bool CRegistryHelper::SetStringValue(LPCSTR lpValueName, LPCSTR lpValue)
{
    if (!m_hKey) return false;
    LONG lRet = RegSetValueExA(
        m_hKey,
        lpValueName,
        0,
        REG_SZ,
        (const BYTE*)lpValue,
        (DWORD)(strlen(lpValue) + 1)); // 包含 '\0'
    return (lRet == ERROR_SUCCESS);
}

bool CRegistryHelper::SetDWORDValue(LPCSTR lpValueName, DWORD dwValue)
{
    if (!m_hKey) return false;
    LONG lRet = RegSetValueExA(
        m_hKey,
        lpValueName,
        0,
        REG_DWORD,
        (const BYTE*)&dwValue,
        sizeof(DWORD));
    return (lRet == ERROR_SUCCESS);
}

bool CRegistryHelper::SetBinaryValue(LPCSTR lpValueName, const BYTE* lpData, DWORD dwSize)
{
    if (!m_hKey) return false;
    LONG lRet = RegSetValueExA(
        m_hKey,
        lpValueName,
        0,
        REG_BINARY,
        lpData,
        dwSize);
    return (lRet == ERROR_SUCCESS);
}

bool CRegistryHelper::GetStringValue(LPCSTR lpValueName, std::string& outValue)
{
    if (!m_hKey) return false;

    DWORD dwType = REG_SZ;
    DWORD dwSize = 0;

    // 第一次调用获取大小
    LONG lRet = RegQueryValueExA(m_hKey, lpValueName, 0, &dwType, NULL, &dwSize);
    if (lRet != ERROR_SUCCESS || dwType != REG_SZ) return false;

    std::vector<char> buffer(dwSize);
    // 第二次调用获取实际数据
    lRet = RegQueryValueExA(m_hKey, lpValueName, 0, &dwType, (LPBYTE)buffer.data(), &dwSize);
    if (lRet != ERROR_SUCCESS) return false;

    outValue.assign(buffer.data());
    return true;
}

bool CRegistryHelper::GetDWORDValue(LPCSTR lpValueName, DWORD& outValue)
{
    if (!m_hKey) return false;

    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(DWORD);
    LONG lRet = RegQueryValueExA(
        m_hKey,
        lpValueName,
        0,
        &dwType,
        (LPBYTE)&outValue,
        &dwSize);
    return (lRet == ERROR_SUCCESS && dwType == REG_DWORD);
}

bool CRegistryHelper::DeleteValue(LPCSTR lpValueName)
{
    if (!m_hKey) return false;
    LONG lRet = RegDeleteValueA(m_hKey, lpValueName);
    return (lRet == ERROR_SUCCESS);
}

bool CRegistryHelper::DeleteKey(HKEY hRootKey, LPCSTR lpSubKey)
{
    LONG lRet = RegDeleteKeyA(hRootKey, lpSubKey);
    return (lRet == ERROR_SUCCESS);
}
#include "pch.h"
#include "CIniOIClass.h"


CIniOIClass::CIniOIClass(const CString& strFilePath)
{
    m_strFilePath = strFilePath;
}

BOOL CIniOIClass::WriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, const CString& strValue)
{
    return ::WritePrivateProfileString(lpszSection, lpszKey, strValue, m_strFilePath);
}

CString CIniOIClass::ReadString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefault)
{
    TCHAR szBuf[1024] = { 0 };
    ::GetPrivateProfileString(lpszSection, lpszKey, lpszDefault, szBuf, 1024, m_strFilePath);
    return CString(szBuf);
}

BOOL CIniOIClass::WriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue)
{
    CString strVal;
    strVal.Format(_T("%d"), nValue);
    return WriteString(lpszSection, lpszKey, strVal);
}

int CIniOIClass::ReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDefault)
{
    CString strVal = ReadString(lpszSection, lpszKey, _T(""));
    if (strVal.IsEmpty()) return nDefault;
    return _ttoi(strVal);
}

BOOL CIniOIClass::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
    return ::WritePrivateProfileString(lpszSection, lpszKey, NULL, m_strFilePath);
}

BOOL CIniOIClass::DeleteSection(LPCTSTR lpszSection)
{
    return ::WritePrivateProfileString(lpszSection, NULL, NULL, m_strFilePath);
}

std::vector<CString> CIniOIClass::GetAllKeys(LPCTSTR lpszSection)
{
    std::vector<CString> keys;

    TCHAR szBuf[4096] = { 0 }; // 存储结果的缓冲区
    DWORD dwRet = ::GetPrivateProfileString(lpszSection, NULL, NULL,
        szBuf, sizeof(szBuf) / sizeof(TCHAR),
        m_strFilePath);

    if (dwRet > 0)
    {
        // 遍历以 '\0' 分隔的字符串
        TCHAR* p = szBuf;
        while (*p != _T('\0'))
        {
            keys.push_back(CString(p));
            p += _tcslen(p) + 1; // 移动到下一个字符串
        }
    }

    return keys;
}

std::vector<CString> CIniOIClass::GetAllSections()
{
    std::vector<CString> sections;

    TCHAR szBuf[4096] = { 0 };
    DWORD dwRet = ::GetPrivateProfileSectionNames(szBuf, sizeof(szBuf) / sizeof(TCHAR), m_strFilePath);

    if (dwRet > 0)
    {
        TCHAR* p = szBuf;
        while (*p != _T('\0'))
        {
            sections.push_back(CString(p));
            p += _tcslen(p) + 1;
        }
    }

    return sections;
}


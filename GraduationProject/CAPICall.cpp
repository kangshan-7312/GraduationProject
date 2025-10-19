#include "pch.h"
#include "CAPICall.h"
#include <windows.h>
#include <winhttp.h>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "winhttp.lib")

CAPICall::CAPICall() {}
CAPICall::~CAPICall() {}

void CAPICall::SetHeader(const std::string& header)
{
    m_header = header;
}

std::string CAPICall::Get(const std::string& url, const std::map<std::string, std::string>& params)
{
    std::string fullUrl = url;
    if (!params.empty())
    {
        fullUrl += "?";
        bool first = true;
        for (auto& p : params)
        {
            if (!first) fullUrl += "&";

            std::string keyUtf8 = MBCSToUTF8(p.first);
            std::string valueUtf8 = MBCSToUTF8(p.second);

            fullUrl += UrlEncode(keyUtf8) + "=" + UrlEncode(valueUtf8);

            first = false;
        }
    }

    //MessageBoxA(NULL, fullUrl.c_str(), "Debug: Full URL", MB_OK); // 显示完整 URL
    return SendRequest(fullUrl, "GET");
}

std::string CAPICall::Post(const std::string& url, const std::map<std::string, std::string>& params)
{
    std::string postData;
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        if (it != params.begin()) postData += "&";
        postData += UrlEncode(it->first) + "=" + UrlEncode(it->second);
    }
    return SendRequest(url, "POST", postData);
}

// 核心请求函数，宽字符API实现
std::string CAPICall::SendRequest(const std::string& url, const std::string& method, const std::string& postData)
{
    std::string result;

    // 解析 URL（宽字符）
    URL_COMPONENTSW urlComp;
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    wchar_t hostName[256] = { 0 };
    wchar_t urlPath[1024] = { 0 };
    urlComp.lpszHostName = hostName;
    urlComp.dwHostNameLength = _countof(hostName);
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = _countof(urlPath);

    std::wstring wUrl = AnsiToWide(url);
    if (!WinHttpCrackUrl(wUrl.c_str(), (DWORD)wUrl.length(), 0, &urlComp))
        return "";

    // 打开 Session
    HINTERNET hSession = WinHttpOpen(L"CAPICall/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if (!hSession) return "";

    // 连接服务器
    HINTERNET hConnect = WinHttpConnect(hSession, hostName, urlComp.nPort, 0);
    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "";
    }

    // 创建请求
    std::wstring wMethod = AnsiToWide(method);
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        wMethod.c_str(),
        urlPath,
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        (urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0)
    );
    if (!hRequest)
    {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }

    BOOL bResults = FALSE;

    if (_stricmp(method.c_str(), "POST") == 0)
    {
        std::string header = m_header.empty() ? "Content-Type: application/x-www-form-urlencoded\r\n" : m_header + "\r\n";
        bResults = WinHttpSendRequest(
            hRequest,
            AnsiToWide(header).c_str(),
            -1L,
            (LPVOID)postData.c_str(),  // 发送 ANSI / UTF-8 数据
            (DWORD)postData.size(),    // 按字节发送，不要 * sizeof(wchar_t)
            (DWORD)postData.size(),
            0
        );
    }
    else
    {
        bResults = WinHttpSendRequest(
            hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0,
            WINHTTP_NO_REQUEST_DATA,
            0,
            0,
            0
        );
    }

    if (bResults && WinHttpReceiveResponse(hRequest, NULL))
    {
        DWORD dwSize = 0;
        do
        {
            DWORD dwDownloaded = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
            if (dwSize == 0) break;

            char* buffer = new char[dwSize];
            if (!buffer) break;

            if (WinHttpReadData(hRequest, buffer, dwSize, &dwDownloaded) && dwDownloaded > 0)
            {
                result.append(buffer, dwDownloaded);
            }

            delete[] buffer;
        } while (dwSize > 0);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return result;
}

// URL编码
std::string CAPICall::UrlEncode(const std::string& str)
{
    std::string encoded;
    char hex[] = "0123456789ABCDEF";

    for (unsigned char c : str)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            encoded += c;
        else
        {
            encoded += '%';
            encoded += hex[(c >> 4) & 0xF];
            encoded += hex[c & 0xF];
        }
    }
    return encoded;
}

// ANSI -> Unicode
std::wstring CAPICall::AnsiToWide(const std::string& str)
{
    if (str.empty()) return L"";
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], len);
    return wstr;
}

// Unicode -> ANSI
std::string CAPICall::WideToAnsi(const std::wstring& wstr)
{
    if (wstr.empty()) return "";
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(len - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], len, NULL, NULL);
    return str;
}

std::string CAPICall::MBCSToUTF8(const std::string& mbcs)
{
    int len = MultiByteToWideChar(CP_ACP, 0, mbcs.c_str(), -1, NULL, 0);
    if (len <= 0) return "";

    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_ACP, 0, mbcs.c_str(), -1, &wstr[0], len);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Len <= 0) return "";

    std::string utf8Str(utf8Len - 1, 0); // 不要末尾 '\0'
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], utf8Len, NULL, NULL);
    return utf8Str;
}

#pragma once
#include <string>
#include <map>

class CAPICall
{
public:
    CAPICall();
    ~CAPICall();

    // 设置自定义请求头，可选
    void SetHeader(const std::string& header);

    // GET请求
    std::string Get(const std::string& url, const std::map<std::string, std::string>& params = {});

    // POST请求
    std::string Post(const std::string& url, const std::map<std::string, std::string>& params);

private:
    std::string m_header; // 可选请求头

    // 核心请求函数
    std::string SendRequest(const std::string& url, const std::string& method, const std::string& postData = "");

    // URL编码
    std::string UrlEncode(const std::string& str);

    // ANSI -> Unicode 转换
    std::wstring AnsiToWide(const std::string& str);

    // Unicode -> ANSI 转换
    std::string WideToAnsi(const std::wstring& wstr);

	// 中文字符串转换为 UTF-8
    std::string MBCSToUTF8(const std::string& mbcs);
};

#pragma once
#include <afx.h>
#include <windows.h>
#include <wincrypt.h>
#include <vector>
#include <string>


#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.lib")


#define BASE64_TEXT   1   // 输入内容为文字
#define BASE64_IMAGE  2   // 输入内容为图片路径

#define SHA_TYPE_TEXT   1   // SHA-文本
#define SHA_TYPE_FILE   2   // SHA-文件



class CCryptocurrency
{
public:
	// 计算MD5哈希并返回CString
	CString GetMD5Hash(CString strText, int nLen = 32, BOOL bUpper = FALSE);


	// Base64 编码
	// param1: base64字符串/图片路径, param2: 类型（BASE64_TEXT:文字类型/BASE64_IMAGE:图片类型）
	CString Base64Encode(CString input, int nType);
	// Base64 解码
    // param1: base64字符串/图片路径, param2:类型（BASE64_TEXT:文字类型/BASE64_IMAGE:图片类型）, param3: 输出路径(文件) or 空(文字)
	CString Base64Decode(CString input, int nType, CString strOutputPath = _T(""));


	// 工具函数：字节转Base64
	CString BytesToBase64(const BYTE* pData, DWORD dwLen);
	// 工具函数：Base64 转字节
	std::vector<BYTE> Base64ToBytes(const CString& base64Str);
	// AES 加密
	// param1: 加密内容, param2: 加密密钥
	CString AESEncrypt(CString strText, CString strKey);
	// AES 解密
	// param1: 解密内容, param2: 解密密钥
	CString AESDecrypt(CString strBase64Cipher, CString strKey);


	// SHA工具函数-字节数组转十六进制字符串
	CString BytesToHexString(const BYTE* data, DWORD len);
	// SHA384 计算
	// param1: 加密内容, param2: 加密类型
	CString GetSHA384(CString input, int nType);
};


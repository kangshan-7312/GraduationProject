#include "pch.h"
#include "CCryptocurrency.h"
#include <string>
#include <vector>


// ==================== MD5 加密 ====================
// param1: 文字内容
// param2: 32 / 64
// param2: 1 / 0（大写/小写）
CString CCryptocurrency::GetMD5Hash(CString strText, int nLen, BOOL bUpper)
{
    // 严格限制参数，只能为 32 或 64
    if (nLen != 32 && nLen != 64)
        return _T("CCryptocurrency::GetMD5Hash参数错误!\nnLen长度只能为64或者32");   // 直接返回空字符串，表示参数错误

    // 获取输入数据(多字节编码转换为字节流)
    CT2A pszConvertedAnsi(strText, CP_ACP);
    const BYTE* pData = (const BYTE*)(LPCSTR)pszConvertedAnsi;
    DWORD dwDataLen = (DWORD)strlen((LPCSTR)pszConvertedAnsi);

    // 初始化CryptoAPI
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE byHash[16];   // MD5固定16字节
    DWORD dwHashLen = sizeof(byHash);
    CString strResult;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return _T("");

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    if (!CryptHashData(hHash, pData, dwDataLen, 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, byHash, &dwHashLen, 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    // 转换成16进制字符串
    CStringA strHex;
    for (DWORD i = 0; i < dwHashLen; i++)
    {
        CStringA tmp;
        tmp.Format("%02x", byHash[i]);  // 小写16进制
        strHex += tmp;
    }

    // 32位：正常输出
    // 64位：输出拼接一遍自身
    CStringA finalHex;
    if (nLen == 32)
        finalHex = strHex;
    else
        finalHex = strHex + strHex;

    // 转换为CString，并处理大小写
    strResult = CA2T(finalHex, CP_ACP);

    if (bUpper)
        strResult.MakeUpper();
    else
        strResult.MakeLower();

    // 释放资源
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return strResult;
}






// ==================== Base64 编码 ====================
// param1: 文字内容/图片路径
// param2: BASE64_TEXT / BASE64_IMAGE
CString CCryptocurrency::Base64Encode(CString input, int nType)
{
    if (nType != BASE64_TEXT && nType != BASE64_IMAGE)
        return _T("");

    BYTE* pData = nullptr;
    DWORD dwDataLen = 0;
    std::vector<BYTE> buffer;

    if (nType == BASE64_TEXT)
    {
        // 转为 ANSI 字节流
        CT2A pszAnsi(input, CP_ACP);
        LPCSTR pAnsi = pszAnsi;
        dwDataLen = (DWORD)strlen(pAnsi);
        buffer.assign((BYTE*)pAnsi, (BYTE*)pAnsi + dwDataLen);
        pData = buffer.data();
    }
    else if (nType == BASE64_IMAGE)
    {
        // 读取文件二进制
        CFile file;
        if (!file.Open(input, CFile::modeRead | CFile::typeBinary))
            return _T("");

        dwDataLen = (DWORD)file.GetLength();
        buffer.resize(dwDataLen);
        file.Read(buffer.data(), dwDataLen);
        file.Close();
        pData = buffer.data();
    }

    // 获取编码后长度
    DWORD dwEncodedLen = 0;
    if (!CryptBinaryToStringA(pData, dwDataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        NULL, &dwEncodedLen))
    {
        return _T("");
    }

    std::string encoded;
    encoded.resize(dwEncodedLen);

    if (!CryptBinaryToStringA(pData, dwDataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        &encoded[0], &dwEncodedLen))
    {
        return _T("");
    }

    // 去掉最后的 '\0'
    if (!encoded.empty() && encoded.back() == '\0')
        encoded.pop_back();

    return CA2T(encoded.c_str(), CP_ACP);
}
// ==================== Base64 解码 ====================
// param1: Base64 字符串/图片路径
// param2: BASE64_TEXT / BASE64_IMAGE
// param3: 输出文件路径(仅在图片模式下有效)
CString CCryptocurrency::Base64Decode(CString input, int nType, CString strOutputPath)
{
    if (nType != BASE64_TEXT && nType != BASE64_IMAGE)
        return _T("");

    CT2A pszAnsi(input, CP_ACP);
    LPCSTR pB64 = pszAnsi;

    // 获取解码后长度
    DWORD dwDecodedLen = 0;
    if (!CryptStringToBinaryA(pB64, 0, CRYPT_STRING_BASE64, NULL, &dwDecodedLen, NULL, NULL))
        return _T("");

    std::vector<BYTE> buffer(dwDecodedLen);

    if (!CryptStringToBinaryA(pB64, 0, CRYPT_STRING_BASE64, buffer.data(), &dwDecodedLen, NULL, NULL))
        return _T("");

    CString result;

    if (nType == BASE64_TEXT)
    {
        // 转回 ANSI 文本
        CStringA strAnsi((LPCSTR)buffer.data(), (int)dwDecodedLen);
        result = CA2T(strAnsi, CP_ACP);
    }
    else if (nType == BASE64_IMAGE)
    {
        // 保存为文件
        CFile file;
        if (!file.Open(strOutputPath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
            return _T("");

        file.Write(buffer.data(), dwDecodedLen);
        file.Close();
        result = strOutputPath;
    }

    return result;
}




// ==================== AES加密工具函数 ====================
CString CCryptocurrency::BytesToBase64(const BYTE* pData, DWORD dwLen)
{
    DWORD dwEncodedLen = 0;
    CryptBinaryToStringA(pData, dwLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &dwEncodedLen);

    std::string encoded;
    encoded.resize(dwEncodedLen);
    CryptBinaryToStringA(pData, dwLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, &encoded[0], &dwEncodedLen);

    if (!encoded.empty() && encoded.back() == '\0')
        encoded.pop_back();

    return CA2T(encoded.c_str(), CP_ACP);
}
// ==================== AES解密工具函数 ====================
std::vector<BYTE> CCryptocurrency::Base64ToBytes(const CString& base64Str)
{
    CT2A ansi(base64Str, CP_ACP);
    std::string s(ansi);

    DWORD dwLen = 0;
    CryptStringToBinaryA(s.c_str(), 0, CRYPT_STRING_BASE64, NULL, &dwLen, NULL, NULL);

    std::vector<BYTE> buffer(dwLen);
    CryptStringToBinaryA(s.c_str(), 0, CRYPT_STRING_BASE64, buffer.data(), &dwLen, NULL, NULL);
    buffer.resize(dwLen);

    return buffer;
}
// ==================== AES加密函数 ====================
// param1: 加密内容
// param2: 加密密钥
CString CCryptocurrency::AESEncrypt(CString strText, CString strKey)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HCRYPTKEY hKey = 0;
    CString strResult;

    // 初始化 CSP
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        return _T("");

    // 创建哈希对象（用来处理密钥）
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    // 把密钥字符串喂入哈希
    CT2A keyAnsi(strKey, CP_ACP);
    if (!CryptHashData(hHash, (BYTE*)(LPCSTR)keyAnsi, (DWORD)strlen((LPCSTR)keyAnsi), 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    // 生成 AES256 对称密钥
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    CryptDestroyHash(hHash);

    // 待加密数据
    CT2A ansiText(strText, CP_ACP);
    std::vector<BYTE> buffer(strlen((LPCSTR)ansiText) + 1);
    memcpy(buffer.data(), (LPCSTR)ansiText, buffer.size());

    DWORD dwDataLen = (DWORD)buffer.size();
    DWORD dwBufLen = dwDataLen + 16; // AES 需要额外空间
    buffer.resize(dwBufLen);

    // 执行加密
    if (!CryptEncrypt(hKey, 0, TRUE, 0, buffer.data(), &dwDataLen, dwBufLen))
    {
        CryptDestroyKey(hKey);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    buffer.resize(dwDataLen);

    // 转成Base64字符串返回
    strResult = BytesToBase64(buffer.data(), dwDataLen);

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);

    return strResult;
}
// ==================== AES解密函数 ====================
// param1: 解密内容
// param2: 解密密钥
CString CCryptocurrency::AESDecrypt(CString strBase64Cipher, CString strKey)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HCRYPTKEY hKey = 0;
    CString strResult;

    // 初始化 CSP
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        return _T("");

    // 创建哈希对象（用于处理密钥）
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    // 把密钥字符串喂入哈希
    CT2A keyAnsi(strKey, CP_ACP);
    if (!CryptHashData(hHash, (BYTE*)(LPCSTR)keyAnsi, (DWORD)strlen((LPCSTR)keyAnsi), 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    // 生成 AES256 对称密钥
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    CryptDestroyHash(hHash);

    // Base64 转二进制
    std::vector<BYTE> buffer = Base64ToBytes(strBase64Cipher);

    DWORD dwDataLen = (DWORD)buffer.size();

    // 执行解密
    if (!CryptDecrypt(hKey, 0, TRUE, 0, buffer.data(), &dwDataLen))
    {
        CryptDestroyKey(hKey);
        CryptReleaseContext(hProv, 0);
        return _T("解密失败!");
    }

    buffer.resize(dwDataLen);

    // 转CString（ANSI → MBCS）
    strResult = CA2T((LPCSTR)buffer.data(), CP_ACP);

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);

    return strResult;
}




// ==================== SHA工具函数 ====================
CString CCryptocurrency::BytesToHexString(const BYTE* data, DWORD len)
{
    CString strResult;
    for (DWORD i = 0; i < len; i++)
    {
        CString temp;
        temp.Format(_T("%02x"), data[i]); // 小写十六进制
        strResult += temp;
    }
    return strResult;
}
// ==================== SHA加密 ====================
CString CCryptocurrency::GetSHA384(CString input, int nType)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    CString strResult;

    // 获取 CSP
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        return _T("");
    }

    // 创建 SHA384 哈希
    if (!CryptCreateHash(hProv, CALG_SHA_384, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        return _T("");
    }

    if (nType == SHA_TYPE_TEXT)
    {
        // 文本模式
        CStringA ansiStr(input); // 转 ANSI
        CryptHashData(hHash, (BYTE*)(LPCSTR)ansiStr, (DWORD)ansiStr.GetLength(), 0);
    }
    else if (nType == SHA_TYPE_FILE)
    {
        // 文件模式
        CFile file;
        if (!file.Open(input, CFile::modeRead | CFile::shareDenyNone))
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return _T("");
        }

        const DWORD BUFFER_SIZE = 8192;
        BYTE buffer[BUFFER_SIZE];
        UINT bytesRead = 0;

        while ((bytesRead = file.Read(buffer, BUFFER_SIZE)) > 0)
        {
            CryptHashData(hHash, buffer, bytesRead, 0);
        }

        file.Close();
    }

    // 获取哈希值
    DWORD dwHashLen = 0;
    DWORD dwLen = sizeof(DWORD);
    CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&dwHashLen, &dwLen, 0);

    std::vector<BYTE> hashBuffer(dwHashLen);
    CryptGetHashParam(hHash, HP_HASHVAL, hashBuffer.data(), &dwHashLen, 0);

    // 转为十六进制字符串
    strResult = BytesToHexString(hashBuffer.data(), dwHashLen);

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return strResult;
}

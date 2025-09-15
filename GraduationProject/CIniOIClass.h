#pragma once
#include <vector>
class CIniOIClass
{
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="strFilePath">INI 文件完整路径</param>
	CIniOIClass(const CString& strFilePath);
	/// <summary>
	/// 写入字符串到 INI 文件
	/// </summary>
	/// <param name="lpszSection">节名，例如 "User"</param>
	/// <param name="lpszKey">键名，例如 "Name"</param>
	/// <param name="strValue">要写入的值，例如 "张三"</param>
	/// <returns>成功返回 TRUE，失败返回 FALSE</returns>
	BOOL WriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, const CString& strValue);
	/// <summary>
	/// 从 INI 文件读取字符串
	/// </summary>
	/// <param name="lpszSection">节名，例如 "User"</param>
	/// <param name="lpszKey">键名，例如 "Name"</param>
	/// <param name="lpszDefault">如果没找到键时的默认返回值</param>
	/// <returns>读取到的字符串</returns>
	CString ReadString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefault = _T(""));
	/// <summary>
	/// 写入整数到 INI 文件
	/// </summary>
	/// <param name="lpszSection">节名，例如 "User"</param>
	/// <param name="lpszKey">键名，例如 "Age"</param>
	/// <param name="nValue">要写入的整数值，例如 25</param>
	/// <returns>成功返回 TRUE，失败返回 FALSE</returns>
	BOOL WriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue);
	/// <summary>
	/// 从 INI 文件读取整数
	/// </summary>
	/// <param name="lpszSection">节名，例如 "User"</param>
	/// <param name="lpszKey">键名，例如 "Age"</param>
	/// <param name="nDefault">如果没找到键时的默认整数值</param>
	/// <returns>读取到的整数值</returns>
	int ReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDefault = 0);
	/// <summary>
	/// 删除指定键
	/// </summary>
	/// <param name="lpszSection">节名</param>
	/// <param name="lpszKey">键名</param>
	/// <returns>成功返回 TRUE，失败返回 FALSE</returns>
	BOOL DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey);
	/// <summary>
	/// 删除整个 Section
	/// </summary>
	/// <param name="lpszSection">节名</param>
	/// <returns>成功返回 TRUE，失败返回 FALSE</returns>
	BOOL DeleteSection(LPCTSTR lpszSection);
	/// <summary>
	/// 获取指定 Section 下的所有键名
	/// </summary>
	/// <param name="lpszSection">节名</param>
	/// <returns>键名数组（CString 向量）</returns>
	std::vector<CString> GetAllKeys(LPCTSTR lpszSection);
	/// <summary>
	/// 获取 INI 文件中所有 Section 名
	/// </summary>
	/// <returns>Section 名数组（CString 向量）</returns>
	std::vector<CString> GetAllSections();
private:
	CString m_strFilePath;
};


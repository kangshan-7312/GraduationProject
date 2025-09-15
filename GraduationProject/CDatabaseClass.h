#pragma once
#include <afx.h>
#include <mysql.h>
#include <string>
#include <vector>

#pragma comment(lib, "libmysql.lib")


// 结果集结构
struct MySQLRow
{
    std::vector<CString> columns;
};

class CDatabaseClass
{
public:
    CDatabaseClass() : m_conn(NULL) {}
    ~CDatabaseClass() { Close(); }

    // 连接数据库
    bool Connect(const CString& host, const CString& user, const CString& passwd, const CString& db, unsigned int port = 3306);
    // 关闭连接
    void Close();
    // 执行 INSERT/UPDATE/DELETE
    bool ExecuteNonQuery(const CString& sql);
    // 查询数据
    bool ExecuteQuery(const CString& sql, std::vector<MySQLRow>& results);
    // Utf8 To MBCS
    CString UTF8ToMBCS(const char* pszUTF8);
    //ConvertMBCS 转 UTF8
    CStringA ConvertMBCSToUTF8(const CString& str);



private:
    MYSQL* m_conn;
};


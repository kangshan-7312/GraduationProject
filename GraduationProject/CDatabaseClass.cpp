#include "pch.h"
#include "CDatabaseClass.h"



//================ 连接数据库 ===================
bool CDatabaseClass::Connect(const CString& host, const CString& user, const CString& passwd, const CString& db, unsigned int port)
{
    m_conn = mysql_init(NULL);
    if (!m_conn) return false;

    CStringA h(host), u(user), p(passwd), d(db);
    if (!mysql_real_connect(m_conn, h, u, p, d, port, NULL, 0))
    {
        return false;
    }
    mysql_set_character_set(m_conn, "utf8");
    return true;
}


//================ 关闭连接 ===================
void CDatabaseClass::Close()
{
    if (m_conn)
    {
        mysql_close(m_conn);
        m_conn = NULL;
    }
}


//==============执行 INSERT/UPDATE/DELETE============
bool CDatabaseClass::ExecuteNonQuery(const CString& sql)
{
    if (!m_conn) return false;
    CStringA query(sql);
    if (mysql_query(m_conn, query))
        return false;
    return true;
}


//====================== 查询数据 ===================
bool CDatabaseClass::ExecuteQuery(const CString& sql, std::vector<MySQLRow>& results)
{
    results.clear();
    if (!m_conn) return 0;

    CStringA query(sql);
    if (mysql_query(m_conn, query))
        return 0;

    MYSQL_RES* res = mysql_store_result(m_conn);
    if (!res) return 0;

    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res)))
    {
        MySQLRow r;
        for (int i = 0; i < num_fields; i++)
        {
            if (row[i])
                r.columns.push_back(UTF8ToMBCS(row[i])); // 使用转换函数
            else
                r.columns.push_back(_T(""));
        }
        results.push_back(r);
    }

    mysql_free_result(res);
    return 1;
}

//================ UTF8 TO MBCS ===============
CString CDatabaseClass::UTF8ToMBCS(const char* pszUTF8)
{
    if (!pszUTF8) return _T("");

    // 先 UTF-8 -> 宽字符
    int wlen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, NULL, 0);
    if (wlen <= 0) return _T("");

    std::vector<wchar_t> wbuffer(wlen);
    MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, wbuffer.data(), wlen);

    // 再 宽字符 -> ANSI (MBCS)
    int alen = WideCharToMultiByte(CP_ACP, 0, wbuffer.data(), -1, NULL, 0, NULL, NULL);
    if (alen <= 0) return _T("");

    std::vector<char> abuffer(alen);
    WideCharToMultiByte(CP_ACP, 0, wbuffer.data(), -1, abuffer.data(), alen, NULL, NULL);

    return CString(abuffer.data());
}
//================ ConvertMBCS 转 UTF8 ===============
CStringA CDatabaseClass::ConvertMBCSToUTF8(const CString& str)
{
    // MBCS -> 宽字符
    int wlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    std::vector<wchar_t> wbuf(wlen);
    MultiByteToWideChar(CP_ACP, 0, str, -1, wbuf.data(), wlen);

    // 宽字符 -> UTF-8
    int u8len = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, NULL, 0, NULL, NULL);
    std::vector<char> u8buf(u8len);
    WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, u8buf.data(), u8len, NULL, NULL);

    return CStringA(u8buf.data());
}

#include "pch.h"
#include "CToken.h"

CString CToken::GetSystemTime()
{
    COleDateTime dt = COleDateTime::GetCurrentTime();

    CString strTime;
    strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
        dt.GetYear(), dt.GetMonth(), dt.GetDay(),
        dt.GetHour(), dt.GetMinute(), dt.GetSecond());

    return strTime;
}

CString CToken::AddSeconds(const CString& strTime, int secondsToAdd)
{
    // 输入格式: yyyy-MM-dd HH:mm:ss
    COleDateTime dt;
    if (dt.ParseDateTime(strTime, VAR_DATEVALUEONLY) == FALSE)
    {
        return _T("Invalid Time Format");
    }

    // 转换成 COleDateTimeSpan (时间差类)
    COleDateTimeSpan span(0, 0, 0, secondsToAdd);

    COleDateTime result = dt + span;

    CString strResult;
    strResult.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
        result.GetYear(), result.GetMonth(), result.GetDay(),
        result.GetHour(), result.GetMinute(), result.GetSecond());

    return strResult;
}

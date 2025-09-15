#pragma once
#include <afx.h>   // CString
#include <atlcomtime.h> // COleDateTime


class CToken
{
public:
    // 获取当前系统时间: yyyy-MM-dd HH:mm:ss
    static CString GetSystemTime();

    // 时间加法：传入时间字符串(yyyy-MM-dd HH:mm:ss)，传入要加的秒数
    static CString AddSeconds(const CString& strTime, int secondsToAdd);
};


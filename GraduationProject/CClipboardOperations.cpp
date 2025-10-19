#include "pch.h"
#include "CClipboardOperations.h"

BOOL CClipboardOperations::SetClipboardText(const CString& strText)
{
    // 1. 尝试打开剪贴板（参数 NULL 表示当前进程窗口）
    if (!OpenClipboard(NULL))
        return FALSE;

    // 2. 清空剪贴板中的原有内容
    EmptyClipboard();

    // 3. 为即将写入的文本分配全局内存（GMEM_MOVEABLE 是剪贴板要求的内存类型）
    int nLen = (strText.GetLength() + 1) * sizeof(TCHAR);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nLen);
    if (!hGlobal)
    {
        CloseClipboard();
        return FALSE;
    }

    // 4. 将文本内容复制到分配的内存中
    LPTSTR pText = (LPTSTR)GlobalLock(hGlobal);
    if (!pText)
    {
        GlobalFree(hGlobal);
        CloseClipboard();
        return FALSE;
    }
    memcpy(pText, strText.GetString(), nLen);
    GlobalUnlock(hGlobal);

    // 5. 设置剪贴板数据格式（UNICODE 或 ANSI）
#ifdef _UNICODE
    SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
    SetClipboardData(CF_TEXT, hGlobal);
#endif

    // 6. 关闭剪贴板
    CloseClipboard();
    return TRUE;
}

CString CClipboardOperations::GetClipboardText()
{
    CString strText;

    // 1. 打开剪贴板
    if (!OpenClipboard(NULL))
        return _T("");

    // 2. 根据字符集获取对应格式的数据句柄
#ifdef _UNICODE
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
#else
    HANDLE hData = GetClipboardData(CF_TEXT);
#endif

    if (hData)
    {
        // 3. 锁定内存并读取文本
        LPCTSTR pText = (LPCTSTR)GlobalLock(hData);
        if (pText)
        {
            strText = pText;
            GlobalUnlock(hData);
        }
    }

    // 4. 关闭剪贴板
    CloseClipboard();
    return strText;
}

BOOL CClipboardOperations::ModifyClipboardToUppercase()
{
    // 1. 先读取当前剪贴板内容
    CString oldText = GetClipboardText();
    if (oldText.IsEmpty())
        return FALSE;

    // 2. 文本处理，比如转大写
    oldText.MakeUpper();

    // 3. 写回剪贴板
    return SetClipboardText(oldText);
}

#pragma once

/**
 * @class CClipboardOperations
 * @brief 封装剪贴板操作（读取、写入、修改等）
 *
 * 使用方式：
 *   CClipboardOperations::SetClipboardText(_T("测试文本"));
 *   CString text = CClipboardOperations::GetClipboardText();
 *   CClipboardOperations::ModifyClipboardToUppercase();
 */
class CClipboardOperations
{
public:
    /**
     * @brief 设置文本到剪贴板（会覆盖原有内容）
     * @param strText 要写入的文本
     * @return TRUE 写入成功；FALSE 打开或设置失败
     */
    static BOOL SetClipboardText(const CString& strText);

    /**
     * @brief 从剪贴板读取文本
     * @return 读取到的文本内容；失败返回空字符串
     */
    static CString GetClipboardText();

    /**
     * @brief 示例功能：读取剪贴板文本 → 转为大写 → 写回剪贴板
     * @return TRUE 成功；FALSE 读取或写入失败
     */
    static BOOL ModifyClipboardToUppercase();
};


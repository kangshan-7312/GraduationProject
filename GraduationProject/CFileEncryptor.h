#pragma once
#include <afx.h>
#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <atlconv.h>   // 用于 CA2W 转换宏

// 定义进度回调和完成回调类型
// 进度回调参数：百分比 (0~100)
// 完成回调参数：true=成功，false=失败
using ProgressCallback = std::function<void(int)>;
using CompletionCallback = std::function<void(bool)>;


#pragma once
#include <afx.h>
#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <atlconv.h>   // 用于 CA2W 转换宏

// 定义进度回调和完成回调类型
// 进度回调参数：百分比 (0~100)
// 完成回调参数：true=成功，false=失败
using ProgressCallback = std::function<void(int)>;
using CompletionCallback = std::function<void(bool)>;

class CFileEncryptor
{
public:
    CFileEncryptor();
    ~CFileEncryptor();

    // 异步加密函数
    // 参数:
    //   srcPath     - 源文件路径 (CString)
    //   password    - 加密密码 (CString，可选)
    //   progress    - 进度回调函数
    //   completion  - 完成回调函数
    // 返回值:
    //   true = 成功启动线程
    //   false = 已有任务在执行
    bool EncryptFileAsync(const CString& srcPath,
        const CString& password,
        const ProgressCallback& progress,
        const CompletionCallback& completion);
    // 异步解密函数
    // 参数:
    //   srcPath     - 待解密文件路径 (CString)
    //   password    - 解密密码 (CString，可选)
    //   progress    - 进度回调函数
    //   completion  - 完成回调函数
    bool DecryptFileAsync(const CString& srcPath,
        const CString& password,
        const ProgressCallback& progress,
        const CompletionCallback& completion);

private:
    std::thread m_worker;                  // 后台线程
    std::atomic<bool> m_running;           // 标记是否正在运行
    ProgressCallback m_progressCb;         // 进度回调
    CompletionCallback m_completionCb;     // 完成回调

    // 实际执行加密的函数 (在后台线程中运行)
    void WorkerEncrypt(std::wstring srcFile, std::wstring password);
};
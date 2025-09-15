#include "pch.h"
#include "CFileEncryptor.h"
#include <fstream>
#include <vector>


CFileEncryptor::CFileEncryptor()
    : m_running(false)
{
}

CFileEncryptor::~CFileEncryptor()
{
    // 确保线程结束再析构
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

// 启动异步加密
bool CFileEncryptor::EncryptFileAsync(const CString& srcPath,
    const CString& password,
    const ProgressCallback& progress,
    const CompletionCallback& completion)
{
    if (m_running.load())
        return false; // 已有任务执行中

    m_running.store(true);
    m_progressCb = progress;
    m_completionCb = completion;

    // 将 CString 转换为 std::wstring (兼容 MBCS / Unicode)
    std::wstring wsrc(CA2W(srcPath.GetString()));
    std::wstring wpwd(CA2W(password.GetString()));

    // 启动后台线程
    m_worker = std::thread(&CFileEncryptor::WorkerEncrypt, this, wsrc, wpwd);
    m_worker.detach(); // 分离线程，不阻塞主线程

    return true;
}

bool CFileEncryptor::DecryptFileAsync(const CString& srcPath, const CString& password, const ProgressCallback& progress, const CompletionCallback& completion)
{
    if (m_running.load())
        return false; // 已有任务执行中

    m_running.store(true);
    m_progressCb = progress;
    m_completionCb = completion;

    // 多字节字符集下转换
    std::wstring wsrc(CA2W(srcPath.GetString()));
    std::wstring wpwd(CA2W(password.GetString()));

    // 启动后台线程
    m_worker = std::thread([this, wsrc, wpwd]()
        {
            bool success = false;
            try
            {
                std::fstream file(wsrc, std::ios::in | std::ios::out | std::ios::binary);
                if (!file.is_open())
                    throw std::runtime_error("文件无法打开");

                // 读取整个文件
                file.seekg(0, std::ios::end);
                size_t fileSize = static_cast<size_t>(file.tellg());
                file.seekg(0, std::ios::beg);

                std::vector<char> buffer(fileSize);
                file.read(buffer.data(), fileSize);

                // XOR 解密 (同加密)
                size_t pwdLen = wpwd.length();
                for (size_t i = 0; i < fileSize; ++i)
                {
                    buffer[i] ^= static_cast<char>(wpwd[i % pwdLen]);

                    if (i % (fileSize / 100 + 1) == 0 && m_progressCb)
                    {
                        int percent = static_cast<int>((i * 100) / fileSize);
                        m_progressCb(percent);
                    }
                }

                // 写回覆盖原文件
                file.seekp(0, std::ios::beg);
                file.write(buffer.data(), fileSize);
                file.close();

                success = true;
            }
            catch (...)
            {
                success = false;
            }

            // 结束，重置状态
            m_running.store(false);

            // 回调
            if (m_completionCb)
                m_completionCb(success);

        });

    m_worker.detach();
    return true;
}

// 实际加密逻辑
void CFileEncryptor::WorkerEncrypt(std::wstring srcFile, std::wstring password)
{
    bool success = false;
    try
    {
        // 打开文件 (读写模式，二进制方式)
        std::fstream file(srcFile, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("文件无法打开");

        // 读取整个文件到内存
        file.seekg(0, std::ios::end);
        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(fileSize);
        file.read(buffer.data(), fileSize);

        // 简单加密逻辑：按字节 XOR 密码
        size_t pwdLen = password.length();
        for (size_t i = 0; i < fileSize; ++i)
        {
            buffer[i] ^= static_cast<char>(password[i % pwdLen]);

            // 每处理 1% 通知进度
            if (i % (fileSize / 100 + 1) == 0 && m_progressCb)
            {
                int percent = static_cast<int>((i * 100) / fileSize);
                m_progressCb(percent);
            }
        }

        // 回到文件开头，写回加密数据 (覆盖原文件)
        file.seekp(0, std::ios::beg);
        file.write(buffer.data(), fileSize);
        file.close();

        success = true;
    }
    catch (...)
    {
        success = false;
    }

    // 任务结束，重置状态
    m_running.store(false);

    // 回调通知完成
    if (m_completionCb)
    {
        m_completionCb(success);
    }
}
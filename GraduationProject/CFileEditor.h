#pragma once
#include <afx.h>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>

using ProgressCallback = std::function<void(int)>;
using CompletionCallback = std::function<void(bool, const CString&)>;

class CFileEditor
{
public:
    CFileEditor();
    ~CFileEditor();

    // 异步读取文件
    bool ReadFileAsync(const CString& filePath,
        const ProgressCallback& progress = nullptr,
        const CompletionCallback& completion = nullptr);

    // 异步保存文件
    bool SaveFileAsync(const CString& filePath,
        const ProgressCallback& progress = nullptr,
        const CompletionCallback& completion = nullptr);

    CString GetContent() const;
    void SetContent(const CString& content);

private:
    std::thread m_worker;
    std::atomic<bool> m_running;
    ProgressCallback m_progressCb;
    CompletionCallback m_completionCb;
    CString m_content;
    CString m_filePath;

    void WorkerRead(CString filePath);
    void WorkerSave(CString filePath);
    bool DetectUTF8(const std::vector<char>& buffer);
};


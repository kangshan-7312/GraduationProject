#include "pch.h"
#include "CFileEditor.h"
#include <fstream>
#include <sstream>

CFileEditor::CFileEditor() : m_running(false) {}
CFileEditor::~CFileEditor()
{
    if (m_worker.joinable())
        m_worker.join();
}

CString CFileEditor::GetContent() const { return m_content; }
void CFileEditor::SetContent(const CString& content) { m_content = content; }

bool CFileEditor::ReadFileAsync(const CString& filePath,
    const ProgressCallback& progress,
    const CompletionCallback& completion)
{
    if (m_running.load()) return false;

    m_running.store(true);
    m_progressCb = progress;
    m_completionCb = completion;
    m_filePath = filePath;

    m_worker = std::thread(&CFileEditor::WorkerRead, this, filePath);
    m_worker.detach();
    return true;
}

bool CFileEditor::SaveFileAsync(const CString& filePath,
    const ProgressCallback& progress,
    const CompletionCallback& completion)
{
    if (m_running.load()) return false;

    m_running.store(true);
    m_progressCb = progress;
    m_completionCb = completion;

    m_worker = std::thread(&CFileEditor::WorkerSave, this, filePath);
    m_worker.detach();
    return true;
}

// 检测 UTF-8
bool CFileEditor::DetectUTF8(const std::vector<char>& buffer)
{
    size_t i = 0;
    while (i < buffer.size())
    {
        unsigned char c = buffer[i];
        if (c <= 0x7F)
            i++;
        else if ((c & 0xE0) == 0xC0 && i + 1 < buffer.size() && (buffer[i + 1] & 0xC0) == 0x80)
            i += 2;
        else if ((c & 0xF0) == 0xE0 && i + 2 < buffer.size()
            && (buffer[i + 1] & 0xC0) == 0x80
            && (buffer[i + 2] & 0xC0) == 0x80)
            i += 3;
        else
            return false;
    }
    return true;
}

// 异步读取
void CFileEditor::WorkerRead(CString filePath)
{
    bool success = false;
    CString result;

    try
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("无法打开文件");

        file.seekg(0, std::ios::end);
        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        const size_t blockSize = 64 * 1024;
        std::vector<char> buffer(blockSize);
        std::ostringstream oss;

        size_t totalRead = 0;
        bool isUTF8 = true;

        while (file)
        {
            file.read(buffer.data(), blockSize);
            std::streamsize readCount = file.gcount();
            if (readCount <= 0) break;

            if (totalRead == 0)
                isUTF8 = DetectUTF8(std::vector<char>(buffer.begin(), buffer.begin() + readCount));

            if (isUTF8)
            {
                // UTF-8 -> ANSI
                std::string tmp(buffer.data(), static_cast<size_t>(readCount));
                int wlen = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), static_cast<int>(tmp.size()), nullptr, 0);
                if (wlen > 0)
                {
                    std::vector<wchar_t> wstr(wlen);
                    MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), static_cast<int>(tmp.size()), wstr.data(), wlen);

                    int len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), wlen, nullptr, 0, nullptr, nullptr);
                    if (len > 0)
                    {
                        std::vector<char> mb(len);
                        WideCharToMultiByte(CP_ACP, 0, wstr.data(), wlen, mb.data(), len, nullptr, nullptr);
                        oss.write(mb.data(), len);
                    }
                }
            }
            else
            {
                oss.write(buffer.data(), static_cast<size_t>(readCount));
            }

            totalRead += static_cast<size_t>(readCount);
            if (m_progressCb)
                m_progressCb(static_cast<int>((totalRead * 100) / fileSize));
        }

        file.close();
        result = CA2T(oss.str().c_str());
        m_content = result;
        success = true;
    }
    catch (...)
    {
        success = false;
        result = _T("读取失败");
    }

    m_running.store(false);
    if (m_completionCb)
        m_completionCb(success, result);
}

// 异步保存
void CFileEditor::WorkerSave(CString filePath)
{
    bool success = false;
    try
    {
        std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
            throw std::runtime_error("无法打开文件");

        LPCSTR content = m_content.GetString();   // MBCS 工程直接使用 char*
        size_t totalSize = strlen(content);

        const size_t blockSize = 64 * 1024;
        size_t written = 0;

        while (written < totalSize)
        {
            size_t writeCount = (blockSize < (totalSize - written)) ? blockSize : (totalSize - written);
            file.write(content + written, writeCount);
            written += writeCount;

            if (m_progressCb)
                m_progressCb(static_cast<int>((written * 100) / totalSize));
        }

        file.close();
        success = true;
    }
    catch (...)
    {
        success = false;
    }

    m_running.store(false);
    if (m_completionCb)
        m_completionCb(success, success ? _T("保存成功") : _T("保存失败"));
}

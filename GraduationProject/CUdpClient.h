#pragma once
#include <afx.h>
#include <afxsock.h>
#include <thread>
#include <atomic>
#include <functional>
#include <fstream>


/// <summary>
/// UDP 文件/消息 客户端类
/// 支持：发送消息、发送文件、接收文件（异步）
/// </summary>
class CUdpClient
{
public:
    using ProgressCB = std::function<void(int)>;          // 进度回调：百分比
    using LogCB = std::function<void(const CString&)>;    // 日志回调

    CUdpClient() : m_running(false), m_sock(INVALID_SOCKET) {}
    ~CUdpClient() { Stop(); }

    /// <summary>
    /// 启动 UDP 客户端
    /// </summary>
    bool Start(const CString& serverIp, UINT serverPort, const CString& saveDir, LogCB log = nullptr);
    /// <summary>
    /// 停止 UDP 客户端
    /// </summary>
    void Stop();
    /// <summary>
    /// 发送文本消息
    /// </summary>
    void SendMessage(const CString& msg);
    /// <summary>
    /// 异步发送文件
    /// </summary>
    void SendFile(const CString& filePath, ProgressCB progress = nullptr);
private:
    /// <summary>
    /// 接收文件循环（后台线程）
    /// </summary>
    void RecvLoop();

private:
    SOCKET m_sock;
    sockaddr_in m_server;
    std::atomic<bool> m_running;
    std::thread m_recvThread;

    CString m_saveDir;   // 文件保存目录
    LogCB m_log;
};


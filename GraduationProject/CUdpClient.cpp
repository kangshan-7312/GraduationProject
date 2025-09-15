#include "pch.h"
#include "CUdpClient.h"

bool CUdpClient::Start(const CString& serverIp, UINT serverPort, const CString& saveDir, LogCB log)
{
    if (m_running.load()) return false;

    AfxSocketInit(NULL);
    m_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sock == INVALID_SOCKET) return false;

    m_server.sin_family = AF_INET;
    m_server.sin_port = htons(serverPort);
    m_server.sin_addr.s_addr = inet_addr(CT2A(serverIp));

    m_running = true;
    m_saveDir = saveDir;
    m_log = log;

    // 启动接收线程
    m_recvThread = std::thread([this]() { this->RecvLoop(); });

    return true;
}

void CUdpClient::Stop()
{
    m_running = false;
    if (m_sock != INVALID_SOCKET)
    {
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
    if (m_recvThread.joinable())
        m_recvThread.join();
}

void CUdpClient::SendMessage(const CString& msg)
{
    std::string data = CT2A(msg, CP_UTF8); // 使用UTF-8编码
    sendto(m_sock, data.c_str(), (int)data.size(), 0,
        (sockaddr*)&m_server, sizeof(m_server));
}

void CUdpClient::SendFile(const CString& filePath, ProgressCB progress)
{
    std::thread([=]() {
        std::ifstream ifs(CT2A(filePath), std::ios::binary);
        if (!ifs.is_open())
        {
            if (m_log) m_log(_T("文件打开失败"));
            return;
        }

        ifs.seekg(0, std::ios::end);
        size_t fileSize = (size_t)ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        const size_t BUF_SIZE = 4096;
        char buf[BUF_SIZE];
        size_t totalSent = 0;
        int seq = 0;

        while (m_running && ifs)
        {
            ifs.read(buf, BUF_SIZE);
            std::streamsize n = ifs.gcount();
            if (n <= 0) break;

            // 包格式：[seq(4字节)][数据...]
            std::vector<char> packet(sizeof(int) + n);
            memcpy(packet.data(), &seq, sizeof(int));
            memcpy(packet.data() + sizeof(int), buf, n);

            sendto(m_sock, packet.data(), (int)packet.size(), 0,
                (sockaddr*)&m_server, sizeof(m_server));

            totalSent += n;
            seq++;

            int percent = (int)((totalSent * 100) / fileSize);
            if (progress) progress(percent);
        }

        if (m_log) m_log(_T("文件发送完成"));
        }).detach();
}

void CUdpClient::RecvLoop()
{
    char buf[8192];
    sockaddr_in from;
    int fromLen = sizeof(from);

    CString saveFile = m_saveDir + _T("\\recv_file.bin");
    std::ofstream ofs(CT2A(saveFile), std::ios::binary);

    while (m_running)
    {
        int ret = recvfrom(m_sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromLen);
        if (ret > 0)
        {
            ofs.write(buf, ret);
        }
    }

    ofs.close();
    if (m_log) m_log(_T("接收线程退出"));
}



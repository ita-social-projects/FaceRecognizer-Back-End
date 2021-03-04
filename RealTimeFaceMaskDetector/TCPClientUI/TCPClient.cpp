#include "TCPClient.h"

constexpr int EMPTY_FLAGS = 0;
constexpr int DEFAULT_SERVICE_PROVIDER_PROTOCOL = 0;
std::string g_ip;
int g_port;
Qt::TransformationMode g_video_quality;
bool TCPClient::CreateSocket()
{
    if (ignore_calls) 
    {
        return 0;
    }
    WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
    m_soket_info.sin_family = AF_INET;
    m_soket_info.sin_addr.s_addr = inet_addr(g_ip.c_str());
    m_soket_info.sin_port = htons(g_port);
    m_socket = socket(AF_INET, SOCK_STREAM, EMPTY_FLAGS);
    if (m_socket == INVALID_SOCKET)
    {
        return INVALID_SOCKET;
    }
    return 0;
}
int TCPClient::Connect()
{
    if (ignore_calls)
    {
        return 0;
    }
    if (connect(m_socket, (sockaddr*)&m_soket_info, sizeof(m_soket_info)) == SOCKET_ERROR) {
        return WSAGetLastError();
    }
    return 0;
}
int TCPClient::SendBinaryMessage(std::vector<char>& buffer)
{
    if (ignore_calls)
    {
        return 0;
    }
    if (buffer.empty())
    {
        return -1;
    }
    std::string buffer_size_s = std::to_string(buffer.size());

    {
        std::lock_guard<std::mutex> lock(send_mutex);
        // Sending count of bytes to the server.
        if (send(m_socket, buffer_size_s.c_str(), buffer_size_s.length(), EMPTY_FLAGS) == SOCKET_ERROR) {
            return WSAGetLastError();
        }
        // Sending image bytes to the server.
        if (send(m_socket, buffer.data(), buffer.size(), EMPTY_FLAGS) == SOCKET_ERROR) {
            return WSAGetLastError();
        }
    }
    return 0;
}
int TCPClient::CloseSocket()
{
    if (ignore_calls)
    {
        return 0;
    }

    if (closesocket(m_socket) != SOCKET_ERROR)
    {
        WSACleanup();
        return true;
    }
    else
    {
        return false;
    }
}

void TCPClient::Ignore() {
    ignore_calls = true;
}
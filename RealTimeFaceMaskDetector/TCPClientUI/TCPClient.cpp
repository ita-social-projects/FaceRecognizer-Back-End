#include "TCPClient.h"

constexpr int EMPTY_FLAGS = 0;
constexpr int DEFAULT_SERVICE_PROVIDER_PROTOCOL = 0;

std::string g_ip;
int g_port;

bool TCPClient::CreateSocket()
{
    WSAStartup(MAKEWORD(2, 2), &m_wsa_data);

    m_soket_info.sin_family = AF_INET;
    m_soket_info.sin_addr.s_addr = inet_addr(g_ip.c_str());
    m_soket_info.sin_port = htons(g_port);

    m_socket = socket(AF_INET, SOCK_STREAM, EMPTY_FLAGS);

    if (m_socket == INVALID_SOCKET)
    {
        return false;
    }

    return true;
}

bool TCPClient::Connect()
{
    return connect(m_socket, (sockaddr*)&m_soket_info, sizeof(m_soket_info)) != SOCKET_ERROR;
}

bool TCPClient::SendBinaryMessage(std::vector<char>& buffer)
{
    if (buffer.empty())
    {
        return false;
    }

    std::string buffer_size_s = std::to_string(buffer.size());
  
    // Sending count of bytes to the server.
    send(m_socket, buffer_size_s.c_str(), buffer_size_s.length(), EMPTY_FLAGS);

    // Sending image bytes to the server.
    return send(m_socket, buffer.data(), buffer.size(), EMPTY_FLAGS);
}

bool TCPClient::CloseSocket()
{
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

#include "TCPClient.h"

std::string g_ip;
int g_port;

bool TCPClient::CreateSocket()
{
    WSAStartup(MAKEWORD(2, 2), &m_wsa_data);

    m_soket_info.sin_family = AF_INET;
    m_soket_info.sin_addr.s_addr = inet_addr(g_ip.c_str());
    m_soket_info.sin_port = htons(g_port);

    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socket == INVALID_SOCKET)
        return false;
    return true;
}

bool TCPClient::Connect()
{
    if (connect(m_socket, (sockaddr*)&m_soket_info, sizeof(m_soket_info)) != SOCKET_ERROR)
        return true;
    else
        return false;
}

bool TCPClient::ConvertImageToBinary(std::ifstream& image, std::vector<char>& buffer)
{
    image.open("PNG_transparency_demonstration_1.png", std::ios::in | std::ios::binary);

    char byte_image = '\0';

    while (!image.eof())
    {
        byte_image = image.get();
        m_size++;
        buffer.push_back(byte_image);
    }
    m_size--;

    image.close();

    return true;
}

bool TCPClient::SendBinaryMessage(const std::vector<char>& buffer)
{
    std::string size = std::to_string(m_size);
    send(m_socket, size.c_str(), size.length(), 0);

    if (send(m_socket, buffer.data(), buffer.size() - 1, 0))
        return true;
    else
        return false;
}

bool TCPClient::CloseSocket()
{
    if (!closesocket(m_socket) == SOCKET_ERROR)
    {
        WSACleanup();
        return true;
    }
    else
        return false;
}
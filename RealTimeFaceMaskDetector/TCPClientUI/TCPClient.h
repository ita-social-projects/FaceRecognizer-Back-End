#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <fstream>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

extern std::string g_ip;
extern int g_port;

class TCPClient
{
public:
    bool CreateSocket();
    bool Connect();
    bool ConvertImageToBinary(std::ifstream& image, std::vector<char>& buffer);
    bool SendBinaryMessage(const std::vector<char>& buffer);
    bool CloseSocket();

private:
    WSAData m_wsa_data;
    sockaddr_in m_soket_info;
    SOCKET m_socket;
    size_t m_size = 0;
};
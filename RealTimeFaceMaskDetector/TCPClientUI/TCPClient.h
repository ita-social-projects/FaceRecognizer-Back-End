#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <fstream>
#include <vector>

#include "TCPClientUI.h"

#include <QWidget>
#include <QImage>
#include <QDebug>

#include "MaskRecognizer.h"

#pragma comment (lib, "ws2_32.lib")

class FaceRecognitionUI;

extern std::string g_ip;
extern int g_port;

class TCPClient
{
public:
    bool CreateSocket();
    bool Connect();
    bool ConvertImageToBinary(std::ifstream& image, std::vector<char>& buffer);
    bool SendBinaryMessage(std::vector<char>& buffer);
    bool CloseSocket();

    std::unique_ptr<FaceRecognitionUI> m_face_recognition_ui;
private:
    WSAData m_wsa_data;
    sockaddr_in m_soket_info;
    SOCKET m_socket;
    size_t m_size;    
};


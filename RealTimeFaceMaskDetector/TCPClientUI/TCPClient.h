#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <fstream>
#include <vector>
#include <mutex>

#include "TCPClientUI.h"
#include "FaceRecognizer.h"

#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <QtCore/QBuffer>

#include <opencv2/imgcodecs.hpp>

#pragma comment (lib, "ws2_32.lib")

/* Forward declaration. */
class FaceRecognitionUI;

/* These are global variables.
   g_ip stores server ip & g_port stores server port
   g_video_quality stores mode for quality.
   They are defined it TCPClient.cpp file. */
extern std::string g_ip;
extern int g_port;
extern Qt::TransformationMode g_video_quality;

/* This class is designed to interact with server via socket,
   to convert image into binary and send it to the server. */
class TCPClient
{
public:
    /* Initiates use of the Winsock DLL by a process
    and creates socket to reach out server socket. */
    int CreateSocket();
    /* Connects to the server socket. */
    int Connect();
    /* Sends vector of bytes to the server. */
    int SendBinaryMessage(std::vector<char>& buffer);
    /* Diconnects from the server and terminates use of the Winsock DLL  */
    int CloseSocket();

    void Ignore();

private:
    WSAData m_wsa_data;
    sockaddr_in m_soket_info;
    SOCKET m_socket;
    bool ignore_calls = false;
    std::mutex send_mutex;


};
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

#include "ui_FaceRecognitionUI.h"
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

class FaceRecognitionUI : public QWidget
{
    Q_OBJECT

public:
    FaceRecognitionUI(QWidget* parent = Q_NULLPTR);
    ~FaceRecognitionUI();

    bool m_exit_button_clicked = false;
    void Recognize(TCPClient& client);
    QImage Mat2QImage(cv::Mat const& src);
    friend std::vector<char> GetImageBytesVector();

public slots:
    void onExitButtonClicked();

public:
    Ui::FaceRecognitionUI ui;
};

extern std::unique_ptr<MaskRecognizer> m_mask_recognizer;
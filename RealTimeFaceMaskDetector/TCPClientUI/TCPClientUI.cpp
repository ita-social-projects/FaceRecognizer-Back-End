#include "TCPClientUI.h"
#include "TCPClient.h"

#include "FaceRecognitionUI.h"

#include <QtGui/QRegExpValidator>
#include <QtWidgets/qmessagebox>

/* These are global variables.
   g_ip stores server ip & g_port stores server port.
   They are defined in TCPClient.cpp file. */
extern std::string g_ip;
extern int g_port;

TCPClientUI::TCPClientUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    const QIcon winIcon("icon.png");
    this->setWindowIcon(winIcon);

    const QString ip_range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    const QRegExp ip_regex("^" + ip_range
        + "\\." + ip_range
        + "\\." + ip_range
        + "\\." + ip_range + "$");
    const QRegExp port_regex("[1-9]\\d{4}");
    auto* portValidator = new QRegExpValidator(port_regex, this);
    auto* ipValidator = new QRegExpValidator(ip_regex, this);
    ui.Port->setValidator(portValidator);
    ui.Port->setCursorPosition(0);
    ui.IP->setValidator(ipValidator);
    ui.IP->setCursorPosition(0);

    connect(ui.Close, &QPushButton::clicked, this, &TCPClientUI::Close);
    connect(ui.Save, &QPushButton::clicked, this, &TCPClientUI::Save);

    const QString default_ip = "127.0.0.1";
    const QString default_port = "27015";

    ui.IP->setText(default_ip);
    ui.Port->setText(default_port);
}

void TCPClientUI::Save()
{
    const QString ip = ui.IP->text();
    const QString port = ui.Port->text();
    QMessageBox msgBox;

    if (ip.isEmpty())
    {
        msgBox.setText("Please write server ip address!");
        msgBox.exec();
    }
    else if (port.isEmpty())
    {
        msgBox.setText("Please write server listen port!");
        msgBox.exec();
    }
    else
    {
        g_ip = ip.toStdString();
        g_port = ui.Port->text().toInt();

        TCPClient client;
        
        client.CreateSocket();
        
        client.Connect();

        Sleep(1000);
        std::unique_ptr<FaceRecognitionUI> m_face_recognition_ui;
        m_face_recognition_ui = std::make_unique<FaceRecognitionUI>();
        m_face_recognition_ui->show();
        this->hide();
        m_face_recognition_ui->updateWindow(client);
        client.CloseSocket();
    }
}

void TCPClientUI::Close()
{
    close();
}

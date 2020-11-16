#include "TCPClientUI.h"
#include "TCPClient.h"

extern std::string g_ip;
extern int g_port;

TCPClientUI::TCPClientUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QIcon winIcon("icon.png");
    this->setWindowIcon(winIcon);

    QString ip_range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ip_regex("^" + ip_range
        + "\\." + ip_range
        + "\\." + ip_range
        + "\\." + ip_range + "$");
    QRegExpValidator* ipValidator = new QRegExpValidator(ip_regex, this);
    ui.IP->setValidator(ipValidator);

    connect(ui.Close, &QPushButton::clicked, this, &TCPClientUI::Close);
    connect(ui.Save, &QPushButton::clicked, this, &TCPClientUI::Save);
}

void TCPClientUI::Save()
{
    QString ip = ui.IP->text();
    QString port = ui.Port->text();
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

        std::ifstream image;
        std::vector<char> buffer;

        client.CreateSocket();
        client.ConvertImageToBinary(image, buffer);
        client.Connect();

        Sleep(1000);
        client.SendBinaryMessage(buffer);

        client.CloseSocket();
    }
}

void TCPClientUI::Close()
{
    close();
}
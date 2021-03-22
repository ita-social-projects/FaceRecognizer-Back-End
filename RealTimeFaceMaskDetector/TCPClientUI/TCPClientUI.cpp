#include "TCPClientUI.h"
#include "TCPClient.h"
#include "XMLParser.h"

#include "FaceRecognitionUI.h"

#include <QtGui/QRegExpValidator>
#include <QtWidgets/qmessagebox>

#define CONFIG_FILE "config.xml"

/* These are global variables.
   g_ip stores server ip & g_port stores server port
   g_video_quality stores mode for quality.
   They are defined in TCPClient.cpp file. */
extern std::string g_ip;
extern int g_port;
extern Qt::TransformationMode g_video_quality;


TCPClientUI::TCPClientUI(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    const QIcon winIcon("icon.png");
    this->setWindowIcon(winIcon);

    //creating validators for IP and port textEdits on ui
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

    //connecting functions to buttons
    connect(ui.Close, &QPushButton::clicked, this, &TCPClientUI::Close);
    connect(ui.Save, &QPushButton::clicked, this, &TCPClientUI::Save);

    //reading data about ip and port from config file
    std::unique_ptr<ConfigReader> parser = std::make_unique<XMLParser>(CONFIG_FILE);

    auto std_string_ip = parser->GetParam("Client", "ip");
    auto std_string_port = parser->GetParam("Client", "port");
    const QString default_ip = QString::fromStdString(std_string_ip);
    const QString default_port = QString::fromStdString(std_string_port);

    //setting default ip and port on ui
    ui.IP->setText(default_ip);
    ui.Port->setText(default_port);

    //setting g_video_quality based on user choice
    if (ui.radioButton->isChecked())
    {
        g_video_quality = Qt::TransformationMode::FastTransformation;
    }
    if (ui.radioButton_2->isChecked())
    {
        g_video_quality = Qt::TransformationMode::SmoothTransformation;
    }
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

        //modifying message box with new butoons
        msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Close | QMessageBox::Retry);
        
        try
        {
            if (client.CreateSocket())
            {
                throw "Cannot create socket";
            }
            if (client.Connect())
            {
                throw "Cannot connect to server";
            }
        }
        catch (const char* e) {
            msgBox.setText(e);
            //waiting for user choice
            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Ignore:
                break;
            case QMessageBox::Close:
                Close();
                return;
                break;
            case QMessageBox::Retry:
                return;
                break;
            default:
                break;
            }
        } 

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

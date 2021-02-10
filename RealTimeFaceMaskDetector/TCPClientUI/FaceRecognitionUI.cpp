#pragma once
#include "FaceRecognitionUI.h"
#include <QDebug>
#include <thread>

FaceRecognitionUI::FaceRecognitionUI(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.exit_button, &QPushButton::clicked, this, &FaceRecognitionUI::onExitButtonClicked);
}

void FaceRecognitionUI::onExitButtonClicked()
{
    m_exit_button_clicked = true;
    run_analizer = false;
    close();
    qDebug() << "QQQQ!!!";
};

void FaceRecognitionUI::updateWindow(TCPClient& client)
{

    std::thread thrd(&FaceRecognitionUI::recognize, this, 0);

    while (!m_exit_button_clicked)
    {
        /*cv::VideoCapture m_camera;
        if (!m_camera.open(0))
        {
            throw std::runtime_error("can't load camera");
        }*/
        cv::Mat image;
        faceInfo faces;
        //m_camera >> image;

        img_data.GetData(image, faces);

        if (image.empty())
        {
            continue;
        }

        QImage frame = mat2QImage(image);
        QPixmap map = QPixmap::fromImage(frame.scaled(640, 480, Qt::KeepAspectRatio, Qt::FastTransformation));
        ui.frame->setPixmap(map);
        ui.frame->show();
        cv::waitKey(30);

        /*std::vector<char> buffer;
        cv::imwrite("image_face.png", image);

        std::ifstream image_face_send;
        client.ConvertImageToBinary(image_face_send, buffer);
        client.SendBinaryMessage(buffer);

        sendImage();*/

        //Sleep(2000);

        qDebug() << "QQQQ\n   ";
    }
}

void FaceRecognitionUI::recognize(int camera_id)
{
    FaceRecognizer recognizer(img_data, camera_id);

    while (run_analizer)
    {
        recognizer.runAnalysis();
    }
}

QImage FaceRecognitionUI::mat2QImage(cv::Mat const& src)
{
    cv::Mat temp; // make the same cv::Mat
    cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation 
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

void FaceRecognitionUI::sendImage()
{
    
}


FaceRecognitionUI::~FaceRecognitionUI()
{
}

#pragma once
#include "FaceRecognitionUI.h"
#include <QtCore/QDebug>
#include <QtGui/QPainter>
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
    thrd.join();
    close();
    qDebug() << "QQQQ!!!";
};

void FaceRecognitionUI::updateWindow(TCPClient& client)
{
    
    thrd  = std::thread(&FaceRecognitionUI::recognize, this, 0);

    auto painter = new QPainter(this);
    painter->setPen(Qt::green);

    while (!m_exit_button_clicked)
    {
        /*cv::VideoCapture m_camera;
        if (!m_camera.open(0))
        {
            throw std::runtime_error("can't load camera");
        }*/
        cv::Mat image;
        faceInfo faces;
   
        //������� ����, ��� �������� � FaceRecognizer
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
      

        for (auto& face : faces) {
            if (!face.second) {
                qDebug() << "ebalo\n";
                painter->drawRect(face.first.tl().x, face.first.br().y,
                    face.first.br().x- face.first.tl().x,
                    face.first.tl().y - face.first.br().y);
            }
        }


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
    FaceRecognizer recognizer(camera_id);

    while (run_analizer)
    {
        // ���� ���������� FaceRecognitionUI::ImageData � FaceRecognizer, 
        // ��� ��� ������ ����� ����, ���������� ���� ������
        recognizer.runAnalysis(img_data);
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

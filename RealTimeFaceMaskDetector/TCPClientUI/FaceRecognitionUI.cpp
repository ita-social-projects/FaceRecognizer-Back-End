#pragma once
#include "FaceRecognitionUI.h"
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <thread>
#include "TimeCounting.h"

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
    // need for same person check
    std::chrono::high_resolution_clock::time_point send_time, new_send_time;
    send_time = get_current_time_fenced();


    thrd  = std::thread(&FaceRecognitionUI::recognize, this, 0);


    while (!m_exit_button_clicked)
    {
        /*cv::VideoCapture m_camera;
        if (!m_camera.open(0))
        {
            throw std::runtime_error("can't load camera");
        }*/

        cv::Mat image;
        faceInfo faces;
        
        //get info from FaceRecognizer
        img_data.GetData(image, faces);

        if (image.empty())
        {
            continue;
        }

        bool is_all_in_mask = true;
        int height, width;
        for (auto& face : faces) 
        {
            if (!face.second)
            {
                cv::Mat face_img(image, face.first);
                height = face.first.tl().y - face.first.br().y;
                width = face.first.br().x - face.first.tl().x;
                auto face_map = QPixmap::fromImage(mat2QImage(face_img).scaled(width, height, Qt::KeepAspectRatio, Qt::FastTransformation));

                // replace with same person check
                new_send_time = get_current_time_fenced();
                if (to_us(new_send_time - send_time) >= 5)
                {
                    send_time = new_send_time;
                    sendImage(client, face_map);
                    qDebug() << "Image_sent\n";
                }
                

                is_all_in_mask = is_all_in_mask && face.second; 

                auto rect_color = face.second == true ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
                cv::rectangle(image,face.first,rect_color, 3, 8, 0);

                qDebug() << "Face:)\n";
            }
        }

        if (is_all_in_mask) 
        {
            FaceRecognizer::SetPanelTextInMask(image);
        }
        else 
        {
            FaceRecognizer::SetPanelTextWithoutMask(image);
        }

        QImage frame = mat2QImage(image);
        QPixmap map = QPixmap::fromImage(frame.scaled(640, 480, Qt::KeepAspectRatio, Qt::FastTransformation));
        ui.frame->setPixmap(map);
        ui.frame->show();

        cv::waitKey(30);

        qDebug() << "QQQQ\n";
    }

}

void FaceRecognitionUI::recognize(int camera_id)
{
    FaceRecognizer recognizer(camera_id);

    while (run_analizer)
    {
        // маємо передавати FaceRecognitionUI::ImageData в FaceRecognizer, 
        // щоб там змінити даний клас, викликавши його сеттер
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

void FaceRecognitionUI::sendImage(TCPClient& client, QPixmap& pixmap)
{
    std::vector<char> buffer;
    client.ConvertImageToBinary(pixmap, buffer);
    client.SendBinaryMessage(buffer);
}


FaceRecognitionUI::~FaceRecognitionUI()
{

}

#pragma once
#include "FaceRecognitionUI.h"
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <thread>
#include "TimeCounting.h"
#include <future>
#include <utility>

#define IDCAM 0

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
    //thrd.join();
    close();
};

void FaceRecognitionUI::updateWindow(TCPClient& client)
{
    // need for same person check
    std::chrono::high_resolution_clock::time_point send_time, new_send_time;
    send_time = get_current_time_fenced();

    //thrd  = std::thread(&FaceRecognitionUI::recognize, this, IDCAM);

    auto shared_face_recognizer = std::make_shared<FaceRecognizer>(IDCAM);
    auto& hz = shared_face_recognizer->runAnalysis();
    cv::Mat image;
    faceInfo faces;
    while (!m_exit_button_clicked)
    {
        image = hz.m_image;
        faces = hz.m_faces;
        auto future = std::async(std::launch::async, &FaceRecognizer::runAnalysis,
            shared_face_recognizer);

        bool is_all_in_mask = true;
        for (auto& face : faces) 
        {
            if (!face.second)
            {
                cv::Mat face_img(image, face.first);
                new_send_time = get_current_time_fenced();
                if (to_us(new_send_time - send_time) >= 5)
                {
                    send_time = new_send_time;
                    //sendImage(client, face_img.clone());
                    std::thread t(&FaceRecognitionUI::sendImage, this, std::ref(client), face_img.clone());
                    t.detach();
                }
                
                is_all_in_mask = is_all_in_mask && face.second; 
                auto rect_color = face.second == true ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
                cv::rectangle(image,face.first,rect_color, 3, 8, 0);
            }
        }
        if (!(faces.empty())) 
        {
            if (is_all_in_mask)
            {
                FaceRecognizer::SetPanelTextInMask(image);
            }
            else
            {
                FaceRecognizer::SetPanelTextWithoutMask(image);
            }
        }

        QImage frame = mat2QImage(image);
        QPixmap map = QPixmap::fromImage(frame.scaled(ui.frame->width(), ui.frame->height(), Qt::KeepAspectRatio, Qt::FastTransformation));
        ui.frame->setPixmap(map);
        ui.frame->show();

        cv::waitKey(30);
        hz = future.get();
    }
}

QImage FaceRecognitionUI::mat2QImage(cv::Mat const& src)
{
    // make the same cv::Mat
    cv::Mat temp;

    // cvtColor Makes a copt, that what i need
    cvtColor(src, temp, cv::COLOR_BGR2RGB);

    QImage dest((const uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    
    // enforce deep copy, see documentation 
    dest.bits(); 

    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

void FaceRecognitionUI::sendImage(TCPClient& client, cv::Mat img)
{
    std::vector<uchar> ubuffer;
    cv::imencode(".png", img.clone(), ubuffer);
    
    std::vector<char> buffer(ubuffer.begin(), ubuffer.end());

    client.SendBinaryMessage(buffer);
}

FaceRecognitionUI::~FaceRecognitionUI()
{

}

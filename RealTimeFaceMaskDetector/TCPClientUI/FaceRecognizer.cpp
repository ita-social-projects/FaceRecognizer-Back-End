#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer(int camera)
{
    // Закоментуємо, бо ми тут робимо лише КОПІЮ FaceRecognitionUI::ImageData
    //m_data = img_data;

    const std::string face_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_frontalface_alt.xml";
    const std::string mouth_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_mouth.xml";
    const std::string nose_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_nose.xml";

    if (!m_face_cascade.load(face_cascade_path)
        || !m_mouth_cascade.load(mouth_cascade_path)
        || !m_nose_cascade.load(nose_cascade_path))
    {
        throw std::runtime_error("can't load cascades");
    }

    if (!m_camera.open(camera))
    {
        throw std::runtime_error("can't load camera");
    }

}

void FaceRecognizer::runAnalysis(ImageData& img_data)
{
    cv::Mat img, grey_img;
    std::vector<cv::Rect> face_rects, mouth_rects, nose_rects;
    faceInfo faces_with_info;

    // load image from camera
    m_camera >> img;

    // convert image to grayscale
    cvtColor(img, grey_img, cv::COLOR_BGR2GRAY);

    // detect faces
    m_face_cascade.detectMultiScale(grey_img, face_rects, 1.1, 4);

    for (auto& face : face_rects)
    {
        m_mouth_cascade.detectMultiScale(cv::Mat(grey_img, face), mouth_rects, 1.5, 5);
        m_nose_cascade.detectMultiScale(cv::Mat(grey_img, face), nose_rects, 1.5, 5);

        faces_with_info.push_back(std::pair(
            face, 
            mouth_rects.size() == 0 && nose_rects.size() == 0));
    }

    img_data.SetData(img, faces_with_info); // setymo infu v FaceRecognitionUI::ImageData
}

void FaceRecognizer::SetPanelTextInMask(cv::Mat& image)
{
    cv::Mat img_copy = image.clone();
    auto m_color = cv::Scalar(0, 255, 0);

    rectangle(image, cv::Point(70, 0), cv::Point(600, 50), m_color, cv::FILLED, 8, 0);
    putText(image, "Thanks for wearing mask :)", cv::Point(150, 30),
            cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
}

void FaceRecognizer::SetPanelTextWithoutMask(cv::Mat& image)
{
    cv::Mat img_copy = image.clone();
    auto m_color = cv::Scalar(0, 0, 255);

    rectangle(image, cv::Point(70, 0), cv::Point(600, 50), m_color, cv::FILLED, 8, 0);
    putText(image, "Put on a mask please!", cv::Point(170, 30),
        cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
}
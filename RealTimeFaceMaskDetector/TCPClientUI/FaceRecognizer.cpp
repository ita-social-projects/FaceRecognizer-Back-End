#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer(ImageData &data, int camera)
{
    m_data = data;

    const std::string face_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    const std::string mouth_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_mouth.xml";
    const std::string nose_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_nose.xml";

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

void FaceRecognizer::runAnalysis()
{
    cv::Mat img;
    std::vector<cv::Rect> face_rects, mouth_rects, nose_rects;
    std::vector<std::pair<cv::Rect, bool>> faces_with_info;

    // load image from camera
    m_camera >> img;

    // convert image to grayscale
    cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // detect faces
    m_face_cascade.detectMultiScale(img, face_rects, 1.1, 4);

    for (auto& face : face_rects)
    {
        m_mouth_cascade.detectMultiScale(cv::Mat(img, face), mouth_rects, 1.5, 5);
        m_nose_cascade.detectMultiScale(cv::Mat(img, face), nose_rects, 1.5, 5);

        faces_with_info.push_back(std::pair(
            face, 
            mouth_rects.size() == 0 && nose_rects.size() == 0));
    }

    m_data.SetData(img, faces_with_info);
}
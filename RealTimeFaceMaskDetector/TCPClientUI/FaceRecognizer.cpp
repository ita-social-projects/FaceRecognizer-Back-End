#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer(int camera)
{
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

HZ& FaceRecognizer::runAnalysis()
{
    // current frame of video stream
    cv::Mat color_img, gray_img;

    // arrays with rectangles of face\mouth\nose location
    std::vector<cv::Rect> face_rects, mouth_rects, nose_rects;

    // vector of pairs <'rectangle with face location',
    //                  'if this face in mask'>
    faceInfo faces_with_info;

    // load frame from camera
    m_camera >> color_img;

    // convert image to grayscale for better recognition
    cv::cvtColor(color_img, gray_img, cv::COLOR_BGR2GRAY);

    // detecting face rectangles and writing them in face_rects vector
    m_face_cascade.detectMultiScale(gray_img, face_rects, 1.1, 4);

    // iterating over each face in current video frame (rectangle of face location)
    for (auto& face : face_rects)
    {
        // cv::Mat() 'cuts' the face from whole frame by the rectangle,
        // then returns cv::Mat image of this face only
        auto face_image = cv::Mat(gray_img, face);
        
        // detecting mouth and nose rectangles in the face image  
        // writing them in mouth_rects and nose_rects respectively
        m_mouth_cascade.detectMultiScale(face_image, mouth_rects, 1.5, 5);
        m_nose_cascade.detectMultiScale(face_image, nose_rects, 1.5, 5);

        // adding location rectangle of face and info about mask's presence on it
        // if we didn't find mouth and nose on the face, so mask is put on
        faces_with_info.push_back(std::pair(
            face, 
            mouth_rects.size() == 0 && nose_rects.size() == 0));
    }
    hz.m_image = color_img;
    hz.m_faces = faces_with_info;
    return hz;
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
#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer()
{
    const std::string face_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_frontalface_alt.xml";
    const std::string mouth_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_mouth.xml";
    const std::string nose_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_nose.xml";

    if (!m_face_cascade.load(face_cascade_path)
        || !m_mouth_cascade.load(mouth_cascade_path)
        || !m_nose_cascade.load(nose_cascade_path))
    {
        throw std::runtime_error("can't load cascades"); //exs
    }

    /*if (!m_camera.open(camera))
    {
        throw std::runtime_error("can't load camera");
    }*/
}

faceInfo FaceRecognizer::runAnalysis(cv::UMat color_img)
{
    // current frame of video stream
    cv::UMat gray_img;

    // arrays with rectangles of face\mouth\nose location
    std::vector<cv::Rect> face_rects, mouth_rects, nose_rects;

    // vector of pairs <'rectangle with face location',
    //                  'if this face in mask'>
    faceInfo faces_with_info;

    // convert image to grayscale for better recognition
    cv::cvtColor(color_img, gray_img, cv::COLOR_BGR2GRAY);

    // detecting face rectangles and writing them in face_rects vector
    m_face_cascade.detectMultiScale(gray_img, face_rects, 1.1, 4);

    // iterating over each face in current video frame (rectangle of face location)
    for (auto& face : face_rects)
    {
        // cv::Mat() 'cuts' the face from whole frame by the rectangle,
        // then returns cv::Mat image of this face only
        auto face_image = cv::UMat(gray_img, face);
        
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
    
    return faces_with_info;
}
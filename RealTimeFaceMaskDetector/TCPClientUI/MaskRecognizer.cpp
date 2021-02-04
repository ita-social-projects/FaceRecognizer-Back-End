#include "MaskRecognizer.h"

MaskRecognizer::MaskRecognizer()
{
    const std::string face_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_frontalface_alt.xml";
    const std::string mouth_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_mouth.xml";
    const std::string nose_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\res\\haarcascades\\haarcascade_mcs_nose.xml";

    LoadCascades(face_cascade_path, mouth_cascade_path, nose_cascade_path);
    int camera_id = 0;
    SetupCamera(camera_id);
}

bool MaskRecognizer::LoadCascades(const std::string& face_cascade_path, const std::string& mouth_cascade_path, const std::string& nose_cascade_path)
{
    if (!m_face_cascade.load(face_cascade_path)
        || !m_mouth_cascade.load(mouth_cascade_path)
        || !m_nose_cascade.load(nose_cascade_path))
        return false;
    return true;
}

bool MaskRecognizer::SetupCamera(const int& device_id)
{
    if (!m_camera.open(device_id))
        return false;
    return true;
}

bool MaskRecognizer::SetPanelText(cv::Mat& image, const bool& face_is_visible)
{
    cv::Mat img_copy = image.clone();

    if (m_color == cv::Scalar(0, 255, 0) && face_is_visible)
    {
        rectangle(image, cv::Point(70, 0), cv::Point(600, 50), m_color, cv::FILLED, 8, 0);
        putText(image, "Thanks for wearing mask :)", cv::Point(150, 30),
            cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
    }
    else
    {
        rectangle(image, cv::Point(70, 0), cv::Point(600, 50), m_color, cv::FILLED, 8, 0);
        putText(image, "Put on a mask please!", cv::Point(170, 30),
            cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
    }
    bool are_equal = true;//std::equal(image.begin<uchar>(), image.end<uchar>(), img_copy.begin<uchar>());
    return are_equal;
}

bool MaskRecognizer::FaceIsVisible(const std::vector<cv::Rect>& faces, const std::vector<cv::Rect>& faces_bw)
{
    if ((faces.size() == 0) && (faces_bw.size() == 0)) return false;
    return true;
}

void MaskRecognizer::StartRecognition()
{
        cv::Mat img, gray_img, bw_img;
        std::vector<cv::Rect> faces, faces_bw, mouths, noses;
        m_camera >> img;
        flip(img, img, 1);
        cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
        int bw_treshold = 80;
        threshold(gray_img, bw_img, bw_treshold, 255, cv::THRESH_BINARY);
        m_face_cascade.detectMultiScale(gray_img, faces, 1.1, 4);
        m_face_cascade.detectMultiScale(bw_img, faces_bw, 1.1, 4);
        for (auto& face : faces)
        {
            m_mouth_cascade.detectMultiScale(gray_img, mouths, 1.5, 5);
            m_nose_cascade.detectMultiScale(gray_img, noses, 1.5, 5);
            if (mouths.size() == 0 || noses.size() == 0) m_color = cv::Scalar(0, 255, 0); else m_color = cv::Scalar(0, 0, 255);
            rectangle(img,
                cv::Point(cvRound(face.x * m_scale), cvRound(face.y * m_scale)),
                cv::Point(cvRound((face.x + face.width - 1) * m_scale), cvRound((face.y + face.height - 1) * m_scale)),
                m_color, 3, 8, 0);

        }

        SetPanelText(img, FaceIsVisible(faces, faces_bw));
        m_image = img;
        
    
}

const cv::Mat& MaskRecognizer::GetImage() const
{
    return m_image;
}

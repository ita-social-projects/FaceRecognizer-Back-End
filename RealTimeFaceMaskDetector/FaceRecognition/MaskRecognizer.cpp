#include "MaskRecognizer.h"

MaskRecognizer::MaskRecognizer()
{
	std::string face_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    std::string mouth_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_mouth.xml";
	std::string nose_cascade_path = "..\\..\\3rdPartyLibs\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_nose.xml";

	LoadCascades(face_cascade_path,mouth_cascade_path,nose_cascade_path);
    int camera_id = 0;
	SetupCamera(camera_id);
}

bool MaskRecognizer::LoadCascades(std::string face_cascade_path, std::string mouth_cascade_path, std::string nose_cascade_path)
{
	if (!m_face_cascade.load(face_cascade_path)
	|| !m_mouth_cascade.load(mouth_cascade_path)
	|| !m_nose_cascade.load(nose_cascade_path)) 
		return false;
	return true;
}

bool MaskRecognizer::SetupCamera(int device_id)
{
	if (!m_camera.open(device_id)) 
		return false;
	return true;
}

void MaskRecognizer::StartRecognition()
{
    for (;;)
    {
        Mat img, gray_img, bw_img;
        std::vector<Rect> faces, faces_bw, mouths, noses;
        m_camera >> img;
        flip(img, img, 1);
        cvtColor(img, gray_img, COLOR_BGR2GRAY);
        int bw_treshold = 80;
        threshold(gray_img, bw_img, bw_treshold, 255, THRESH_BINARY);
        m_face_cascade.detectMultiScale(gray_img, faces, 1.1, 4);
        m_face_cascade.detectMultiScale(bw_img, faces_bw, 1.1, 4);
        //if ((faces.size() == 0) && (faces_bw.size() == 0)) std::cout << "No face found...";
        //if ((faces.size() == 0) && (faces_bw.size() == 1)) std::cout << "Thx for wearing mask";
        for (size_t i = 0; i < faces.size(); ++i)
        {
            m_mouth_cascade.detectMultiScale(gray_img, mouths, 1.5, 5);
            m_nose_cascade.detectMultiScale(gray_img, noses, 1.5, 5);
            if (mouths.size() == 0 || noses.size() == 0) m_color = Scalar(0, 255, 0); else m_color = Scalar(0, 0, 255);
            rectangle(img,
                Point(cvRound(faces[i].x * m_scale), cvRound(faces[i].y * m_scale)),
                Point(cvRound((faces[i].x + faces[i].width - 1) * m_scale), cvRound((faces[i].y + faces[i].height - 1) * m_scale)),
                m_color, 3, 8, 0);

        }
        imshow("Camera", img);
        waitKey(30);
    }
}


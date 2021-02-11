#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <iostream>
#include <conio.h>	
#include <thread>

//test

class MaskRecognizer
{
public:
	MaskRecognizer();
	void StartRecognition();
	const cv::Mat &GetImage() const;

private:
	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_mouth_cascade;
	cv::CascadeClassifier m_nose_cascade;
	cv::Scalar m_color = cv::Scalar(0, 0, 255);
	int m_scale = 1;
	cv::VideoCapture m_camera;
	cv::Mat m_image;

	bool LoadCascades(const std::string& face_cascade_path, const std::string& mouth_cascade_path, const std::string& nose_cascade_path);
	bool SetupCamera(const int& device_id = 0);
	bool SetPanelText(cv::Mat& image, const bool& face_is_visible);
	bool FaceIsVisible(const std::vector<cv::Rect>& faces, const std::vector <cv::Rect > & faces_bw);

};
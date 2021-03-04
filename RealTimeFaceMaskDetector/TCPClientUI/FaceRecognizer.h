#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/ocl.hpp>

#include <vector>
#include <iostream>
#include <conio.h>
#include <thread>

using faceInfo = std::vector<std::pair<cv::Rect, bool>>;

#define RED cv::Scalar(0, 0, 255)
#define GREEN cv::Scalar(0, 255, 0)

class FaceRecognizer
{
public:
	FaceRecognizer();

	faceInfo runAnalysis(cv::UMat color_img);

private:
	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_mouth_cascade;
	cv::CascadeClassifier m_nose_cascade;

	cv::VideoCapture m_camera;
};
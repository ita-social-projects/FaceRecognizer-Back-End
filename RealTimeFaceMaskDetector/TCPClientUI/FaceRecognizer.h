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

#include "ImageData.h"

class FaceRecognizer
{
public:
	FaceRecognizer(ImageData data, int camera=0);

	void RunAnalysis();

private:
	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_mouth_cascade;
	cv::CascadeClassifier m_nose_cascade;

	cv::VideoCapture m_camera;

	ImageData data;

};
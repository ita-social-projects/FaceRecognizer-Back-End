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
	FaceRecognizer(int camera = 0);

	void runAnalysis(ImageData& img_data);

private:
	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_mouth_cascade;
	cv::CascadeClassifier m_nose_cascade;

	cv::VideoCapture m_camera;

	ImageData m_data; // ця штука локальна, вона ніяк не впливає на ImageData в FaceRecognitionUI
};
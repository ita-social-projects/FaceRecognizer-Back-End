#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <iostream>
#include <conio.h>	

using namespace cv;

class MaskRecognizer
{
public:
	MaskRecognizer();
	void StartRecognition();

private:
	CascadeClassifier m_face_cascade;
	CascadeClassifier m_mouth_cascade;
	CascadeClassifier m_nose_cascade;
	Scalar m_color = Scalar(0, 0, 255);
	int m_scale = 1;
	VideoCapture m_camera;

	bool LoadCascades(std::string face_cascade_path, std::string mouth_cascade_path, std::string nose_cascade_path);
	bool SetupCamera(int device_id = 0);

};


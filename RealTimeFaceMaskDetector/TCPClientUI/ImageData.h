#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <iostream>
#include <conio.h>	
#include <thread>
#include <algorithm>

using faceInfo = std::vector<std::pair<cv::Rect, bool>>;

struct HZ
{
	cv::UMat m_image;
	faceInfo m_faces;
};

class ImageData
{
	cv::UMat m_image;
	faceInfo m_faces;
	std::mutex m_mtx;

public:

	void SetData(cv::UMat image, faceInfo faces);

	void GetData(cv::UMat& image, faceInfo &face);

	ImageData& operator= (const ImageData& obj);
};
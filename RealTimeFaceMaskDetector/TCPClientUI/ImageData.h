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

struct FaceWithMaskInfo {
	cv::Rect face;
	bool has_mask;
};

#define faceInfo std::vector<std::pair<cv::Rect, bool>>  // TODO: typedef  ;   struct instead of pair

class ImageData
{
	cv::Mat m_image;
	faceInfo m_faces;
	std::mutex m_mtx;

public:

	void SetData(cv::Mat image, faceInfo faces);

	void GetData(cv::Mat& image, faceInfo& faces);

	ImageData& operator= (const ImageData& obj);
};
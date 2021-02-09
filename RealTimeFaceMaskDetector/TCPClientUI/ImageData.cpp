#include "ImageData.h"

void ImageData::SetData(cv::Mat& image, faceInfo& faces)
{
	std::lock_guard l_g(m_mtx);
	m_image = image;
	m_faces = faces;
}

void ImageData::GetData(cv::Mat& image, faceInfo& faces)
{
	std::lock_guard l_g(m_mtx);
	image = m_image;
	std::copy(m_faces.begin(), m_faces.end(), std::back_inserter(faces));
}
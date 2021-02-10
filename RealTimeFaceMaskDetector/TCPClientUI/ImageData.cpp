#include "ImageData.h"

void ImageData::SetData(cv::Mat image, faceInfo faces)
{
	std::lock_guard l_g(m_mtx);
	m_image = image.clone();
	m_faces = faces;
}

void ImageData::GetData(cv::Mat& image, faceInfo& faces)
{
	std::lock_guard l_g(m_mtx);
	image = m_image.clone();
	std::copy(m_faces.begin(), m_faces.end(), std::back_inserter(faces));
}

ImageData& ImageData::operator= (const ImageData & obj)
{
	std::lock_guard l_g(m_mtx);
	m_image = obj.m_image.clone();
	m_faces = obj.m_faces;
	return *this;
}

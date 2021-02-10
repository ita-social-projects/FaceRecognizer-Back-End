#include "ImageData.h"

void ImageData::SetData(cv::Mat image, cv::Rect face, bool has_mask)
{
	std::lock_guard l_g(m_mtx);
	m_image = image.clone();
	m_face = face;
	m_has_mask = has_mask;
}

void ImageData::GetData(cv::Mat& image, cv::Rect& face, bool& has_mask)
{
	std::lock_guard l_g(m_mtx);
	image = m_image.clone();
	face = m_face;
	has_mask = m_has_mask;
}

ImageData& ImageData::operator= (const ImageData & obj)
{
	std::lock_guard l_g(m_mtx);
	m_image = obj.m_image.clone();
	m_face = obj.m_face;
	m_has_mask = obj.m_has_mask;
	return *this;
}

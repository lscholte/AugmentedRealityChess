// Author:	Liam Scholte
// Created:	4/23/2022 1:58:35 PM
// This file contains the implementations for BlurFilter
// See BlurFilter.h for documentation

#include <Chess/ArView/Filters/BlurFilter.h>

#pragma warning(push, 0)        
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect.hpp>
#pragma warning(pop)

namespace Chess
{
namespace ArView
{
namespace Filters
{
	BlurFilter::~BlurFilter() = default;

	cv::Mat BlurFilter::apply(cv::Mat const& image) const
	{
		cv::Mat outputImage(image.size(), CV_8UC3);
		cv::GaussianBlur(image, outputImage, cv::Size(9, 9), 0.0);
		return outputImage;
	}
}
}
}

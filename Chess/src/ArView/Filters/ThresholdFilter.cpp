// Author:	Liam Scholte
// Created:	4/23/2022 1:56:14 PM
// This file contains the implementations for ThresholdFilter
// See ThresholdFilter.h for documentation

#include <Chess/ArView/Filters/ThresholdFilter.h>

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
	ThresholdFilter::~ThresholdFilter() = default;

	cv::Mat ThresholdFilter::apply(cv::Mat const& image) const
	{
		cv::Mat hsvImage, outputImage;
		cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
		cv::inRange(hsvImage, cv::Scalar(110, 100, 100), cv::Scalar(130, 255, 255), outputImage);
		return outputImage;
	}
}
}
}

// Author:	Liam Scholte
// Created:	4/23/2022 1:52:37 PM
// This file contains the implementations for CompositeFilter
// See CompositeFilter.h for documentation

#include <Chess/ArView/Filters/CompositeFilter.h>

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
	CompositeFilter::CompositeFilter(std::initializer_list<FilterPtr> const& filters)
		: m_filters(filters)
	{}

	CompositeFilter::~CompositeFilter() = default;

	cv::Mat CompositeFilter::apply(cv::Mat const& image) const
	{
		cv::Mat outputImage = image;
		for (auto const& pFilter : m_filters)
		{
			outputImage = pFilter->apply(outputImage);
		}

		return outputImage;
	}
}
}
}

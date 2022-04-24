// Author:	Liam Scholte
// Created:	4/23/2022 3:44:27 PM
// This file contains the implementations for Region
// See Region.h for documentation

#include <Chess/ArView/Region.h>

#pragma warning(push, 0)        
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/types.hpp>
#pragma warning(pop)

namespace Chess
{
namespace ArView
{
	struct Region::Impl
	{
		std::vector<cv::Point2i> points;
		cv::Point2d centroid;

		Impl(std::vector<cv::Point2i> const& points, cv::Point2d const& centroid)
			: points(points)
			, centroid(centroid)
		{}
	};

	Region::Region(std::vector<cv::Point2i> const& points, cv::Point2d const& centroid)
		: m_pImpl(std::make_shared<Impl>(points, centroid))
	{
	}

	Region::~Region() = default;

	std::vector<cv::Point2i> const& Region::getPoints() const
	{
		return m_pImpl->points;
	}

	cv::Point2d const& Region::getCentroid() const
	{
		return m_pImpl->centroid;
	}

	int Region::getArea() const
	{
		return m_pImpl->points.size();
	}
}
}

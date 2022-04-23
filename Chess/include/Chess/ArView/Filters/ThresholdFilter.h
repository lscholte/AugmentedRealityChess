// Author:	Liam Scholte
// Created:	4/23/2022 1:43:31 PM
// This file contains the class definition for ThresholdFilter

#pragma once

#include <Chess/ArView/Filters/Filter.h>

namespace Chess
{
namespace ArView
{
namespace Filters
{
	/// <summary>
	/// A filter that produces a binary (black/white) image.
	/// </summary>
	class ThresholdFilter
		: public Filter
	{
	public:
		virtual ~ThresholdFilter() override;

		virtual cv::Mat apply(cv::Mat const& image) const override;
	};
}
}
}

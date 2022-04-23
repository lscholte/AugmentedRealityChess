// Author:	Liam Scholte
// Created:	4/23/2022 1:46:23 PM
// This file contains the class definition for BlurFilter

#pragma once

#include <Chess/ArView/Filters/Filter.h>

namespace Chess
{
namespace ArView
{
namespace Filters
{
	/// <summary>
	/// A filter that blurs an image.
	/// </summary>
	class BlurFilter
		: public Filter
	{
	public:
		virtual ~BlurFilter() override;

		virtual cv::Mat apply(cv::Mat const& image) const override;
	};
}
}
}

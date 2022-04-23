// Author:	Liam Scholte
// Created:	4/23/2022 1:40:38 PM
// This file contains the class definition for Filter

#pragma once

#include <memory>

namespace cv
{
	class Mat;
}

namespace Chess
{
namespace ArView
{
namespace Filters
{
	/// <summary>
	/// A filter that can be used to apply an effect to an image.
	/// </summary>
	class Filter
	{
	public:
		virtual ~Filter() = default;

		/// <summary>
		/// Applies the effect of this filter to the specified image.
		/// </summary>
		/// <param name="image">The image to which this filter effect should be applied</param>
		/// <returns>An image with a filter effect applied</returns>
		virtual cv::Mat apply(cv::Mat const& image) const = 0;
	};

	using FilterPtr = std::shared_ptr<Filter>;
}
}
}

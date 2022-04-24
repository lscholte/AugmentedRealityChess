// Author:	Liam Scholte
// Created:	4/23/2022 3:29:34 PM
// This file contains the class definition for RegionSegmenter

#pragma once

#include <vector>

namespace cv
{
	class Mat;
}

namespace Chess
{
namespace ArView
{
	class Region;

	/// <summary>
	/// Segments a binary image into disconnected regions.
	/// </summary>
	class RegionSegmenter
	{
	public:

		/// <summary>
		/// Finds a list of regions in the image.
		/// </summary>
		/// <param name="image">The binary image to searh for regions</param>
		/// <returns>A list of regions found in the binary image</returns>
		virtual std::vector<Region> findRegions(cv::Mat const& image) const;
	};

}
}

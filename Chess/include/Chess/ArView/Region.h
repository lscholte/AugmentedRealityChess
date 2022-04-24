// Author:	Liam Scholte
// Created:	4/23/2022 3:15:31 PM
// This file contains the class definition for Region

#pragma once

#include <vector>
#include <memory>

namespace cv
{
	template <typename T>
	class Point_;

	using Point2i = Point_<int>;
	using Point2d = Point_<double>;

	template <typename T, int n>
	class Vec;

	using Vec2d = Vec<double, 2>;
}

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// A collection of connected foreground pixels in an image.
	/// </summary>
	class Region
	{
	public:

		/// <summary>
		/// Constructs a region from a list of points and a centroid.
		/// </summary>
		Region(std::vector<cv::Point2i> const& points, cv::Point2d const& centroid);

		virtual ~Region();

		/// <summary>
		/// Gets the points in the region.
		/// </summary>
		/// <returns>The list of points in the region</returns>
		std::vector<cv::Point2i> const& getPoints() const;

		/// <summary>
		/// Gets the centroid of the region.
		/// </summary>
		/// <returns>The centroid of the region</returns>
		cv::Point2d const& getCentroid() const;

		/// <summary>
		/// Gets the number of pixels in the region.
		/// </summary>
		/// <returns>The number of pixels in the region</returns>
		int getArea() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};

}
}

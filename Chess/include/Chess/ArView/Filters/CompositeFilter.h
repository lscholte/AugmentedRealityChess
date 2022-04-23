// Author:	Liam Scholte
// Created:	4/23/2022 1:48:01 PM
// This file contains the class definition for CompositeFilter

#pragma once

#include <Chess/ArView/Filters/Filter.h>

#include <initializer_list>
#include <vector>

namespace Chess
{
namespace ArView
{
namespace Filters
{
	/// <summary>
	/// A filter that is composed of other filters.
	/// </summary>
	class CompositeFilter
		: public Filter
	{
	public:
		/// <summary>
		/// Constructs a CompoundFilter from a list of other filters.
		/// The order in which filters appear is the order in which they will be applied.
		/// </summary>
		/// <param name="filters">The list of filters to use</param>
		CompositeFilter(std::initializer_list<FilterPtr> const& filters);

		virtual ~CompositeFilter() override;

		virtual cv::Mat apply(cv::Mat const& image) const override;

	private:
		std::vector<FilterPtr> m_filters;
	};

}
}
}

// Author:	Liam Scholte
// Created:	4/23/2022 3:48:16 PM
// This file contains the implementations for RegionSegmenter
// See RegionSegmenter.h for documentation

#include <Chess/ArView/RegionSegmenter.h>
#include <Chess/ArView/Region.h>

#pragma warning(push, 0)        
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect.hpp>
#pragma warning(pop)

#include <numeric>

namespace
{
	int constexpr MIN_REGION_SIZE = 300;
}

namespace Chess
{
namespace ArView
{

	std::vector<Region> RegionSegmenter::findRegions(cv::Mat const& image) const
	{
		cv::Mat labels, stats, centroids;
		int regionCount = cv::connectedComponentsWithStats(image, labels, stats, centroids, 8, CV_32S, cv::CCL_BBDT);

		//Initializes the regions
		std::unordered_map<int, std::vector<cv::Point2i>> regionPoints;

		//Start from region 1 to ignore the background
		for (int regionId = 1; regionId < regionCount; ++regionId)
		{
			//Ignore small regions;
			int area = stats.at<int>(regionId, cv::CC_STAT_AREA);
			if (area < MIN_REGION_SIZE)
			{
				continue;
			}

			regionPoints.insert(std::make_pair(regionId, std::vector<cv::Point2i>()));
		}

		//Adds all the points for the region
		std::vector<cv::Point2i> points;
		for (int row = 0; row < labels.rows; ++row)
		{
			for (int col = 0; col < labels.cols; ++col)
			{
				int regionId = labels.at<int>(row, col);
				auto iter = regionPoints.find(regionId);
				if (iter != regionPoints.end())
				{
					if (row == 0 || row == image.rows - 1 || col == 0 || col == image.cols - 1)
					{
						//Ret rid of regions that touch the edge of the image
						regionPoints.erase(iter);
					}
					else
					{
						//Add the point to the region
						iter->second.emplace_back(col, row);
					}
				}
			}
		}

		std::vector<Region> regionVector;
		regionVector.reserve(regionPoints.size());
		std::transform(
			regionPoints.begin(),
			regionPoints.end(),
			std::back_inserter(regionVector),
			[&centroids](std::pair<int, std::vector<cv::Point2i>> const& item)
			{
				return Region(
					item.second,
					cv::Point2d(centroids.at<double>(item.first, 0), centroids.at<double>(item.first, 1)));
			});

		return regionVector;
	}
}
}

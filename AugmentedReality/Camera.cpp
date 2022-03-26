// Author:	Liam Scholte
// Created:	3/6/2022 1:28:10 PM
// This file contains the implementations for Camera
// See Camera.h for documentation

#include "Camera.h"
#include "ObjectDrawer.h"
#include "Constants.h"

#pragma warning(push, 0)        
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/calib3d.hpp>
#pragma warning(pop) 

#include <glm/ext.hpp>

#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <numeric>

namespace
{
	using ImageCoordinate = cv::Point2f;
	using ImageCoordinateList = std::vector<ImageCoordinate>;

	using WorldCoordinate = cv::Point3f;
	using WorldCoordinateList = std::vector<WorldCoordinate>;

	int constexpr minCalibrationImages = 5;

	cv::Size const checkerboardSize(9, 6);

	WorldCoordinateList generateWorldCoordinates()
	{
		WorldCoordinateList worldCoordinateList;
		for (int y = 0; y < checkerboardSize.height; ++y)
		{
			for (int x = 0; x < checkerboardSize.width; ++x)
			{
				worldCoordinateList.emplace_back(x, -y, 0.0f);
			}
		}

		return worldCoordinateList;
	}
}

struct Camera::Impl
{
	std::mutex mutex;

	cv::VideoCapture videoCapture;

	bool cornersOrdered;
	ImageCoordinateList checkerboardCorners;

	std::vector<ImageCoordinateList> imagePoints;
	std::vector<WorldCoordinateList> objectPoints;

	cv::Size imageSize;

	cv::Mat cameraMatrix;
	std::vector<float> distortionCoefficients;

	bool isCalibrated;
	double reprojectionError;

	ObjectDrawer objectDrawer;

	Impl()
		: videoCapture(0)
		, cornersOrdered(false)
		, imageSize(videoCapture.get(cv::CAP_PROP_FRAME_WIDTH), videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT))
		, objectDrawer(imageSize.width, imageSize.height)
	{
		resetCalibration();
	}

	bool canSaveCalibrationImage()
	{
		return cornersOrdered;
	}

	bool canCalibrate()
	{
		return imagePoints.size() >= minCalibrationImages;
	}

	void resetCalibration()
	{
		reprojectionError = NAN;
		imagePoints.clear();
		objectPoints.clear();
		distortionCoefficients.clear();
		isCalibrated = false;
	}
};

Camera::Camera()
	: m_pImpl(std::make_shared<Impl>())
{
}

Camera::~Camera() = default;

size_t Camera::getWidth() const
{
	return m_pImpl->imageSize.width;
}

size_t Camera::getHeight() const
{
	return m_pImpl->imageSize.height;
}

std::vector<unsigned char> Camera::getImage(bool showCalibrationInfo)
{
	std::scoped_lock lock(m_pImpl->mutex);

	cv::Mat image;
	m_pImpl->videoCapture >> image;

	//Finds the chessboard corners. This is unfortunately a very expensive operation.
	//CALIB_CB_FAST_CHECK seems to help with that, but it is still expensive
	m_pImpl->cornersOrdered = cv::findChessboardCorners(image, checkerboardSize, m_pImpl->checkerboardCorners, cv::CALIB_CB_FAST_CHECK);
	if (m_pImpl->cornersOrdered)
	{
		cv::Mat temp;
		cv::cvtColor(image, temp, cv::COLOR_BGR2GRAY);
		cv::cornerSubPix(temp, m_pImpl->checkerboardCorners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001));
	}

	if (showCalibrationInfo)
	{
		cv::drawChessboardCorners(image, checkerboardSize, m_pImpl->checkerboardCorners, m_pImpl->cornersOrdered);
	}

	if (m_pImpl->isCalibrated)
	{
		//Undistortion is needed such that the OpenGL objects get drawn in the right place
		//because the transformation math is implemented myself rather than via cv::projectPoints.
		//This is unfortunately also an expensive operation. It may be worth investigating
		//how I could handle image distortion in OpenGL
		cv::Mat modified;
		cv::undistort(image, modified, m_pImpl->cameraMatrix, m_pImpl->distortionCoefficients);
		image = modified;
	}

	if (m_pImpl->isCalibrated && m_pImpl->canSaveCalibrationImage())
	{
		WorldCoordinateList worldCoordinateList = generateWorldCoordinates();

		cv::Vec3f rvec, tvec;
		cv::solvePnP(
			worldCoordinateList,
			m_pImpl->checkerboardCorners,
			m_pImpl->cameraMatrix,
			m_pImpl->distortionCoefficients,
			rvec,
			tvec);

		glm::mat4 extrinsic;
		std::memcpy(glm::value_ptr(extrinsic), cv::Affine3(rvec, tvec).matrix.val, 16 * sizeof(float));
		extrinsic = glm::transpose(extrinsic);

		float f = m_pImpl->cameraMatrix.at<double>(0, 0);
		float cx = m_pImpl->cameraMatrix.at<double>(0, 2);
		float cy = m_pImpl->cameraMatrix.at<double>(1, 2);
		float width = m_pImpl->imageSize.width;
		float height = m_pImpl->imageSize.height;

		//From https://amytabb.com/tips/tutorials/2019/06/28/OpenCV-to-OpenGL-tutorial-essentials/
		glm::mat4 intrinsic;
		intrinsic[0][0] = -f;
		intrinsic[0][1] = 0.0;
		intrinsic[0][2] = 0.0;
		intrinsic[0][3] = 0.0;

		intrinsic[1][0] = 0.0;
		intrinsic[1][1] = -f;
		intrinsic[1][2] = 0.0;
		intrinsic[1][3] = 0.0;

		intrinsic[2][0] = (width-cx);
		intrinsic[2][1] = (height-cy);
		intrinsic[2][2] = -(Z_NEAR + Z_FAR);
		intrinsic[2][3] = 1.0;

		intrinsic[3][0] = 0.0;
		intrinsic[3][1] = 0.0;
		intrinsic[3][2] = Z_NEAR * Z_FAR;
		intrinsic[3][3] = 0.0;

		//Draw AR objects into the scene. This will draw xyz axes and a teapot.
		m_pImpl->objectDrawer.draw(image.data, intrinsic * extrinsic);
	}

	//Copy and return the image data
	size_t imageDataSize = getWidth() * getHeight() * image.elemSize();
	std::vector<unsigned char> imageData;
	imageData.resize(imageDataSize);
	std::memcpy(imageData.data(), image.data, imageData.size());

	return imageData;
}

bool Camera::canSaveCalibrationImage() const
{
	std::scoped_lock lock(m_pImpl->mutex);
	return m_pImpl->canSaveCalibrationImage();
}


bool Camera::saveCalibrationImage()
{
	std::scoped_lock lock(m_pImpl->mutex);
	if (!m_pImpl->canSaveCalibrationImage())
	{
		return false;
	}

	m_pImpl->imagePoints.push_back(m_pImpl->checkerboardCorners);

	WorldCoordinateList& worldCoordinateList = m_pImpl->objectPoints.emplace_back();
	worldCoordinateList = generateWorldCoordinates();

	return true;
}

int Camera::getCalibrationImageCount() const
{
	std::scoped_lock lock(m_pImpl->mutex);
	return m_pImpl->imagePoints.size();
}

bool Camera::canCalibrate() const
{
	std::scoped_lock lock(m_pImpl->mutex);
	return m_pImpl->canCalibrate();
}

bool Camera::calibrate()
{
	std::scoped_lock lock(m_pImpl->mutex);
	if (!m_pImpl->canCalibrate())
	{
		return false;
	}

	m_pImpl->cameraMatrix = cv::Mat::zeros(3, 3, CV_64FC1);
	m_pImpl->cameraMatrix.at<double>(0, 0) = 1.0;
	m_pImpl->cameraMatrix.at<double>(1, 1) = 1.0;
	m_pImpl->cameraMatrix.at<double>(2, 2) = 1.0;
	m_pImpl->cameraMatrix.at<double>(0, 2) = 0.5 * m_pImpl->imageSize.width;
	m_pImpl->cameraMatrix.at<double>(1, 2) = 0.5 * m_pImpl->imageSize.height;
	
	std::vector<cv::Mat> rvecs, tvecs;
	m_pImpl->reprojectionError = cv::calibrateCamera(
		m_pImpl->objectPoints,
		m_pImpl->imagePoints,
		m_pImpl->imageSize,
		m_pImpl->cameraMatrix,
		m_pImpl->distortionCoefficients,
		rvecs,
		tvecs,
		cv::CALIB_FIX_ASPECT_RATIO);

	m_pImpl->isCalibrated = true;

	return true;
}

void Camera::resetCalibration()
{
	std::scoped_lock lock(m_pImpl->mutex);
	m_pImpl->resetCalibration();
}

bool Camera::loadCalibrationFromFile(std::string const& fileName)
{
	std::scoped_lock lock(m_pImpl->mutex);

	std::ifstream fileStream;
	fileStream.open(fileName);
	if (!fileStream.is_open())
	{
		return false;
	}

	double reprojectionError;
	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
	std::vector<float> distortionCoefficients;
	distortionCoefficients.resize(5);
	std::vector<ImageCoordinateList> imagePoints;
	std::vector<WorldCoordinateList> objectPoints;
	try
	{
		std::string csvLine;
		std::getline(fileStream, csvLine);
		
		//projErr,fx,fy,cx,cy,k1,k2,p1,p2,k3

		std::string tempItem;
		std::istringstream lineStream(csvLine);

		//Read the first row of data

		std::getline(lineStream, tempItem, ',');
		reprojectionError = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		cameraMatrix.at<double>(0, 0) = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		cameraMatrix.at<double>(1, 1) = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		cameraMatrix.at<double>(0, 2) = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		cameraMatrix.at<double>(1, 2) = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		distortionCoefficients[0] = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		distortionCoefficients[1] = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		distortionCoefficients[2] = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		distortionCoefficients[3] = std::stof(tempItem);

		std::getline(lineStream, tempItem, ',');
		distortionCoefficients[4] = std::stof(tempItem);

		//Read the calibrated image points
		while (std::getline(fileStream, csvLine))
		{
			lineStream = std::istringstream(csvLine);

			//Ignore the first entry which just identifies the calibration image
			std::getline(lineStream, tempItem, ',');

			ImageCoordinateList& points = imagePoints.emplace_back();
			std::string xCoord, yCoord;
			while (std::getline(lineStream, xCoord, ',') && std::getline(lineStream, yCoord, ','))
			{
				points.emplace_back(std::stof(xCoord), std::stof(yCoord));
			}

			//Add corresponding world coordinates
			WorldCoordinateList& worldCoordinateList = objectPoints.emplace_back();
			worldCoordinateList = generateWorldCoordinates();
		}

	}
	catch (std::exception const& e)
	{
		return false;
	}

	m_pImpl->reprojectionError = reprojectionError;
	m_pImpl->cameraMatrix = cameraMatrix;
	m_pImpl->distortionCoefficients = distortionCoefficients;
	m_pImpl->imagePoints = imagePoints;
	m_pImpl->objectPoints = objectPoints;
	m_pImpl->isCalibrated = true;

	return true;
}

bool Camera::saveCalibrationToFile(std::string const& fileName) const
{
	std::scoped_lock lock(m_pImpl->mutex);

	if (!m_pImpl->isCalibrated)
	{
		return false;
	}

	std::ofstream fileStream;
	fileStream.open(fileName);
	if (!fileStream.is_open())
	{
		return false;
	}

	try
	{
		fileStream
			<< m_pImpl->reprojectionError << ","
			<< m_pImpl->cameraMatrix.at<double>(0, 0) << ","
			<< m_pImpl->cameraMatrix.at<double>(1, 1) << ","
			<< m_pImpl->cameraMatrix.at<double>(0, 2) << ","
			<< m_pImpl->cameraMatrix.at<double>(1, 2) << ","
			<< m_pImpl->distortionCoefficients[0] << ","
			<< m_pImpl->distortionCoefficients[1] << ","
			<< m_pImpl->distortionCoefficients[2] << ","
			<< m_pImpl->distortionCoefficients[3] << ","
			<< m_pImpl->distortionCoefficients[4] << "\n";

		for (size_t i = 0; i < m_pImpl->imagePoints.size(); ++i)
		{
			ImageCoordinateList const& imagePointList = m_pImpl->imagePoints[i];
			std::string csvRow = std::accumulate(
				imagePointList.cbegin(),
				imagePointList.cend(),
				std::string("Calibration " + std::to_string(i+1)),
				[](std::string const& csvRow, ImageCoordinate const& imagePoint)
				{
					return csvRow + "," + std::to_string(imagePoint.x) + "," + std::to_string(imagePoint.y);
				});

			fileStream << csvRow << "\n";
		}
	}
	catch (std::exception const& e)
	{
		return false;
	}

	return true;
}

double Camera::getReprojectionError() const
{
	std::scoped_lock lock(m_pImpl->mutex);
	return m_pImpl->reprojectionError;
}
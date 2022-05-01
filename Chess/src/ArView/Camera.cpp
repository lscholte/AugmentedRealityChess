// Author:	Liam Scholte
// Created:	3/6/2022 1:28:10 PM
// This file contains the implementations for Camera
// See Camera.h for documentation

#include <Chess/ArView/Camera.h>
#include <Chess/ArView/ObjectDrawer.h>
#include <Chess/ArView/Constants.h>
#include <Chess/ArView/Filters/Filter.h>
#include <Chess/ArView/Filters/BlurFilter.h>
#include <Chess/ArView/Filters/ThresholdFilter.h>
#include <Chess/ArView/Filters/CompositeFilter.h>

#include <Chess/Controller/Controller.h>

#include <Chess/Model/Position.h>

#pragma warning(push, 0)        
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco/charuco.hpp>
#pragma warning(pop)

#include <glm/ext.hpp>

#include <boost/range/irange.hpp>

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

	using CharucoId = int;
	using CharucoIdList = std::vector<CharucoId>;

	int constexpr minCalibrationImages = 5;

	cv::Size const checkerboardSize(7, 7);
}

namespace Chess
{
namespace ArView
{

	struct Camera::Impl
	{
		std::mutex mutex;

		cv::VideoCapture videoCapture;

		ImageCoordinateList currentCharucoCorners;
		CharucoIdList currentCharucoIds;

		std::vector<ImageCoordinateList> calibrationCharucoCorners;
		std::vector<CharucoIdList> calibrationCharucoIds;

		cv::Size imageSize;

		cv::Mat cameraMatrix;
		std::vector<float> distortionCoefficients;

		bool isCalibrated;
		double reprojectionError;

		std::shared_ptr<Controller::Controller> pController;

		ObjectDrawer objectDrawer;

		cv::Ptr<cv::aruco::Dictionary> charucoDictionary;
		cv::Ptr<cv::aruco::CharucoBoard> charucoBoard;

		Filters::FilterPtr pThresholdFilter;

		std::optional<Model::Position> oPointerPosition;
		unsigned int pointerPositionCounter;

		Impl()
			: videoCapture("http://10.0.0.105/video.mjpg")
			, imageSize(videoCapture.get(cv::CAP_PROP_FRAME_WIDTH), videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT))
			, pController(std::make_shared<Controller::Controller>())
			, objectDrawer(imageSize.width, imageSize.height, pController)
			, pointerPositionCounter(0)
		{
			resetCalibration();

			charucoDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
			charucoBoard = cv::aruco::CharucoBoard::create(8, 8, 1.0f, 0.8f, charucoDictionary);

			std::initializer_list<Filters::FilterPtr> filters
			{
				std::make_shared<Filters::BlurFilter>(),
				std::make_shared<Filters::ThresholdFilter>(),
			};

			pThresholdFilter = std::make_shared<Filters::CompositeFilter>(filters);
		}

		bool canSaveCalibrationImage()
		{
			return !currentCharucoCorners.empty();
		}

		bool canCalibrate()
		{
			return calibrationCharucoCorners.size() >= minCalibrationImages;
		}

		void resetCalibration()
		{
			reprojectionError = NAN;
			calibrationCharucoCorners.clear();
			calibrationCharucoIds.clear();
			distortionCoefficients.clear();
			isCalibrated = false;
		}

		void updatePointerPosition(std::optional<Model::Position> const& oPosition)
		{
			if (oPosition == oPointerPosition)
			{
				++pointerPositionCounter;
			}
			else
			{
				pointerPositionCounter = 0;
			}
			oPointerPosition = oPosition;
		}
	};

	Camera::Camera()
		: m_pImpl(std::make_unique<Impl>())
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

	std::vector<unsigned char> Camera::getImage(bool showCalibrationInfo, bool enableHandThresholding)
	{
		std::scoped_lock lock(m_pImpl->mutex);

		cv::Mat image;
		m_pImpl->videoCapture >> image;

		cv::resize(image, image, m_pImpl->imageSize);

		m_pImpl->currentCharucoCorners.clear();
		m_pImpl->currentCharucoIds.clear();

		cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();
		std::vector<int> markerIds;
		std::vector<ImageCoordinateList> markerCorners;
		cv::aruco::detectMarkers(image, m_pImpl->charucoDictionary, markerCorners, markerIds, params);

		if (!markerCorners.empty())
		{
			//cv::Mat temp;
			//cv::cvtColor(image, temp, cv::COLOR_BGR2GRAY);
			//for (auto& points : markerCorners)
			//{
			//	cv::cornerSubPix(temp, points, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001));
			//}

			cv::aruco::interpolateCornersCharuco(
				markerCorners,
				markerIds,
				image,
				m_pImpl->charucoBoard,
				m_pImpl->currentCharucoCorners,
				m_pImpl->currentCharucoIds);
		}

		if (showCalibrationInfo)
		{
			cv::aruco::drawDetectedMarkers(image, markerCorners, markerIds);
			if (!m_pImpl->currentCharucoCorners.empty())
			{
				cv::aruco::drawDetectedCornersCharuco(image, m_pImpl->currentCharucoCorners, m_pImpl->currentCharucoIds);
			}
		}

		//if (m_pImpl->isCalibrated)
		//{
		//	//Undistortion is needed such that the OpenGL objects get drawn in the right place
		//	//because the transformation math is implemented myself rather than via cv::projectPoints.
		//	//This is unfortunately also an expensive operation. It may be worth investigating
		//	//how I could handle image distortion in OpenGL
		//	cv::Mat modified;
		//	cv::undistort(image, modified, m_pImpl->cameraMatrix, m_pImpl->distortionCoefficients);
		//	image = modified;
		//}

		if (m_pImpl->isCalibrated && m_pImpl->canSaveCalibrationImage())
		{
			cv::Vec3f rvec, tvec;
			bool poseFound = cv::aruco::estimatePoseCharucoBoard(
				m_pImpl->currentCharucoCorners,
				m_pImpl->currentCharucoIds,
				m_pImpl->charucoBoard,
				m_pImpl->cameraMatrix,
				m_pImpl->distortionCoefficients,
				rvec,
				tvec);

			if (poseFound)
			{
				glm::mat4 extrinsic, extrinsicTranspose;
				std::memcpy(glm::value_ptr(extrinsicTranspose), cv::Affine3(rvec, tvec).matrix.val, 16 * sizeof(float));

				//This seems backwards, but it is not.
				//The reason for this is that a GLM matrix is in column major order rather than row major
				extrinsic = glm::transpose(extrinsicTranspose);

				glm::mat3 inverseRotation = -glm::mat3(extrinsicTranspose);
				glm::vec3 translation(extrinsic[3]);
				glm::vec3 cameraPosition = inverseRotation * translation;

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

				intrinsic[2][0] = (width - cx);
				intrinsic[2][1] = (height - cy);
				intrinsic[2][2] = -(Z_NEAR + Z_FAR);
				intrinsic[2][3] = 1.0;

				intrinsic[3][0] = 0.0;
				intrinsic[3][1] = 0.0;
				intrinsic[3][2] = Z_NEAR * Z_FAR;
				intrinsic[3][3] = 0.0;

				cv::Mat thresholdImage;
				if (enableHandThresholding)
				{
					thresholdImage = m_pImpl->pThresholdFilter->apply(image);
					int const count = 5;
					cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
					cv::erode(thresholdImage, thresholdImage, structuringElement, cv::Point(-1, -1), count);
				}
				else
				{
					thresholdImage = cv::Scalar(255);
				}

				//Draw AR objects into the scene. This will draw the chessboard and pieces
				m_pImpl->objectDrawer.draw(image.data, thresholdImage.data, intrinsic * extrinsic, cameraPosition);

				if (enableHandThresholding)
				{
					std::vector<std::vector<cv::Point>> contours;
					std::vector<cv::Vec4i> hierarchy;
					cv::findContours(thresholdImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

					std::sort(
						contours.begin(),
						contours.end(),
						[](std::vector<cv::Point> const& a, std::vector<cv::Point> const& b)
						{
							return cv::contourArea(a) > cv::contourArea(b);
						});

					for (size_t i = 0; i < std::min(size_t(1), contours.size()); ++i)
					{
						cv::Moments m = cv::moments(contours[i]);
						if (m.m00 < 300)
						{
							continue;
						}
						cv::Point centroid(m.m10 / m.m00, m.m01 / m.m00);
						cv::circle(image, centroid, 2, cv::Scalar(0, 255, 0), 5);


						auto iter = std::max_element(
							contours[i].begin(),
							contours[i].end(),
							[&centroid, &imageSize = m_pImpl->imageSize](cv::Point const& a, cv::Point const& b)
						{
							float aDistanceToEdge = std::min({
								a.x,
								a.y,
								imageSize.width - a.x,
								imageSize.height - a.y});
							float aDistanceToCentroid = cv::norm(a - centroid);
							float bDistanceToEdge = std::min({
								b.x,
								b.y,
								imageSize.width - b.x,
								imageSize.height - b.y});
							float bDistanceToCentroid = cv::norm(b - centroid);

							return 
								aDistanceToEdge * aDistanceToCentroid * aDistanceToCentroid < 
								bDistanceToEdge * bDistanceToCentroid * bDistanceToCentroid;
						});

						cv::circle(image, *iter, 2, cv::Scalar(255, 0, 0), 5);
			

						std::optional<Model::Position> oPosition = m_pImpl->objectDrawer.handleClick((float)iter->x / m_pImpl->imageSize.width, (float)iter->y / m_pImpl->imageSize.height);
						if (oPosition)
						{
							std::string text = "(" + std::to_string(oPosition->rank) + "," + std::to_string(oPosition->file) + ")";
							cv::putText(
								image,
								text,
								*iter,
								cv::FONT_HERSHEY_COMPLEX_SMALL,
								1.0,
								cv::Scalar(255, 0, 0));
						}
						m_pImpl->updatePointerPosition(oPosition);

						if (m_pImpl->pointerPositionCounter >= 30)
						{
							if (m_pImpl->oPointerPosition)
							{
								m_pImpl->pController->selectPosition(*m_pImpl->oPointerPosition);
							}
							else
							{
								m_pImpl->pController->unselectPosition();
							}
						}
					}
				}
			}
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

		m_pImpl->calibrationCharucoCorners.push_back(m_pImpl->currentCharucoCorners);
		m_pImpl->calibrationCharucoIds.push_back(m_pImpl->currentCharucoIds);

		return true;
	}

	int Camera::getCalibrationImageCount() const
	{
		std::scoped_lock lock(m_pImpl->mutex);
		return m_pImpl->calibrationCharucoCorners.size();
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
		m_pImpl->reprojectionError = cv::aruco::calibrateCameraCharuco(
			m_pImpl->calibrationCharucoCorners,
			m_pImpl->calibrationCharucoIds,
			m_pImpl->charucoBoard,
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
		std::vector<ImageCoordinateList> calibrationCharucoCorners;
		std::vector<CharucoIdList> calibrationCharucoIds;
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

				ImageCoordinateList& charucoCorners = calibrationCharucoCorners.emplace_back();
				CharucoIdList& charucoIds = calibrationCharucoIds.emplace_back();
				std::string id, xCoord, yCoord;
				while (std::getline(lineStream, id, ',') && std::getline(lineStream, xCoord, ',') && std::getline(lineStream, yCoord, ','))
				{
					charucoIds.emplace_back(std::stoi(id));
					charucoCorners.emplace_back(std::stof(xCoord), std::stof(yCoord));
				}
			}

		}
		catch (std::exception const& e)
		{
			return false;
		}

		m_pImpl->reprojectionError = reprojectionError;
		m_pImpl->cameraMatrix = cameraMatrix;
		m_pImpl->distortionCoefficients = distortionCoefficients;
		m_pImpl->calibrationCharucoCorners = calibrationCharucoCorners;
		m_pImpl->calibrationCharucoIds = calibrationCharucoIds;
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

			for (size_t i = 0; i < m_pImpl->calibrationCharucoCorners.size(); ++i)
			{
				auto cornerIndexRange = boost::irange(m_pImpl->calibrationCharucoCorners[i].size());

				std::string csvRow = std::accumulate(
					cornerIndexRange.begin(),
					cornerIndexRange.end(),
					std::string("Calibration " + std::to_string(i + 1)),
					[this, i](std::string const& csvRow, size_t j)
					{
						CharucoId id = m_pImpl->calibrationCharucoIds[i][j];
						ImageCoordinate cornerPoint = m_pImpl->calibrationCharucoCorners[i][j];
						return csvRow + "," + std::to_string(id) + "," + std::to_string(cornerPoint.x) + "," + std::to_string(cornerPoint.y);
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

	void Camera::handleLeftClick(float x, float y)
	{
		std::scoped_lock lock(m_pImpl->mutex);
		std::optional<Model::Position> oPosition = m_pImpl->objectDrawer.handleClick(x, y);

		if (oPosition)
		{
			m_pImpl->pController->selectPosition(*oPosition);
		}
	}

	void Camera::handleRightClick()
	{
		std::scoped_lock lock(m_pImpl->mutex);
		m_pImpl->pController->unselectPosition();
	}
}
}

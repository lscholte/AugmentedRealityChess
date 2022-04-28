// Author:	Liam Scholte
// Created:	3/6/2022 1:28:10 PM
// This file contains the class definition for Camera

#pragma once

#include <Chess/Macros.h>

#include <string>
#include <vector>
#include <memory>

namespace Chess
{
namespace ArView
{

	/// <summary>
	/// Retrieves and processes images from a camera connected to the computer.
	/// This camera can be calibrated by capturing images of a chessboard.
	/// </summary>
	class EXPORT Camera
	{
	public:

		/// <summary>
		/// Constructs an uncalibrated camera.
		/// </summary>
		Camera();

		virtual ~Camera();

		/// <summary>
		/// Gets the width of the camera's image.
		/// </summary>
		/// <returns>The width of the image</returns>
		size_t getWidth() const;

		/// <summary>
		/// Gets the height of the camera's image.
		/// </summary>
		/// <returns>The height of the image</returns>
		size_t getHeight() const;

		/// <summary>
		/// Retrieves an image from the computer's camera and potentially
		/// applies visual effects to the image.
		/// </summary>
		/// <param name="showCalibrationInfo">Whether or not to show chessboard calibration info</param>
		/// <param name="enableHandThresholding">Whether or not to enable detection of hands</param>
		/// <returns>A vector of binary image data in BGR format</returns>
		std::vector<unsigned char> getImage(bool showCalibrationInfo, bool enableHandThresholding);

		/// <summary>
		/// Determines if the camera currently has a suitable calibration
		/// image that can be saved.
		/// </summary>
		/// <returns>True if the camera can save a calibration image, false otherwise</returns>
		bool canSaveCalibrationImage() const;

		/// <summary>
		/// Saves the current calibration image.
		/// Does nothing if there is no current calibration image.
		/// <returns>True if the camera saved a calibration image, false otherwise</returns>
		/// </summary>
		bool saveCalibrationImage();

		/// <summary>
		/// Gets the number of saved calibration images.
		/// </summary>
		/// <returns>The number of saved calibration images</returns>
		int getCalibrationImageCount() const;

		/// <summary>
		/// Determines if the camera can be calibrated,
		/// which requires a minimum number of calibration images to be saved.
		/// </summary>
		/// <returns>True if the camera can be calibrated, false otherwise</returns>
		bool canCalibrate() const;

		/// <summary>
		/// Calibrates the camera. Does nothing if the camera cannot be calibrated yet.
		/// </summary>
		/// <returns>True if the camera was calibrated, false otherwise</returns>
		bool calibrate();

		/// <summary>
		/// Resets the camera calibration back to its original state.
		/// This leaves the camera with no currently saved calibration images
		/// and no camera calibration.
		/// </summary>
		void resetCalibration();

		/// <summary>
		/// Loads a pre-existing calibration data from a CSV file.
		/// If successful, the camera will be calibrated according to this data.
		/// </summary>
		/// <param name="fileName">The path to the calibration CSV file</param>
		/// <returns>True if the camera was calibrated, false otherwise</returns>
		bool loadCalibrationFromFile(std::string const& fileName);

		/// <summary>
		/// Saves the current calibration data to a CSV file.
		/// Does nothing if there is no current camera calibration.
		/// </summary>
		/// <param name="fileName">The path to the calibration CSV file</param>
		/// <returns>True if the camera calibration was saved, false otherwise</returns>
		bool saveCalibrationToFile(std::string const& fileName) const;

		/// <summary>
		/// Gets the reprojection error of the camera calibration.
		/// </summary>
		/// <returns>The reprojection error if the camera is calibrated, NaN otherwise</returns>
		double getReprojectionError() const;

		void handleLeftClick(float x, float y);

		void handleRightClick();

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}


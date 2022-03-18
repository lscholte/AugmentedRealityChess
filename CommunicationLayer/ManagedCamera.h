// Author:	Liam Scholte
// Created:	3/6/2022 1:23:39 PM
// This file contains the class definition for ManagedCamera

#pragma once

#using <PresentationCore.dll>

class Camera;

/// <summary>
/// Provides a managed wrapper around a native C++ Camera, suitable for use in C#.
/// </summary>
public ref class ManagedCamera
{
public:

	/// <summary>
	/// Constructs a ManagedCamera wrapper around a native C++ Camera.
	/// </summary>
	ManagedCamera();

	virtual ~ManagedCamera();

	/// <summary>
	/// Retrieves an image from the computer's camera and potentially
	/// applies visual effects to the image.
	/// </summary>
	/// <param name="showCalibrationInfo">Whether or not to show chessboard calibration info</param>
	/// <returns>A bitmap source representing the image</returns>
	System::Windows::Media::Imaging::BitmapSource^ GetImage(bool showCalibrationInfo);

	/// <summary>
	/// Determines if the camera currently has a suitable calibration
	/// image that can be saved.
	/// </summary>
	/// <returns>True if the camera can save a calibration image, false otherwise</returns>
	bool CanSaveCalibrationImage();

	/// <summary>
	/// Saves the current calibration image.
	/// Does nothing if there is no current calibration image.
	/// <returns>True if the camera saved a calibration image, false otherwise</returns>
	/// </summary>
	bool SaveCalibrationImage();

	/// <summary>
	/// Gets the number of saved calibration images.
	/// </summary>
	/// <returns>The number of saved calibration images</returns>
	int GetCalibrationImageCount();

	/// <summary>
	/// Determines if the camera can be calibrated,
	/// which requires a minimum number of calibration images to be saved.
	/// </summary>
	/// <returns>True if the camera can be calibrated, false otherwise</returns>
	bool CanCalibrateCamera();

	/// <summary>
	/// Calibrates the camera. Does nothing if the camera cannot be calibrated yet.
	/// </summary>
	/// <returns>True if the camera was calibrated, false otherwise</returns>
	bool CalibrateCamera();

	/// <summary>
	/// Resets the camera calibration back to its original state.
	/// This leaves the camera with no currently saved calibration images
	/// and no camera calibration.
	/// </summary>
	void ResetCalibration();

	/// <summary>
	/// Loads a pre-existing calibration data from a CSV file.
	/// If successful, the camera will be calibrated according to this data.
	/// </summary>
	/// <param name="fileName">The path to the calibration CSV file</param>
	/// <returns>True if the camera was calibrated, false otherwise</returns>
	bool LoadCalibrationFromFile(System::String^ fileName);

	/// <summary>
	/// Saves the current calibration data to a CSV file.
	/// Does nothing if there is no current camera calibration.
	/// </summary>
	/// <param name="fileName">The path to the calibration CSV file</param>
	/// <returns>True if the camera calibration was saved, false otherwise</returns>
	bool SaveCalibrationToFile(System::String^ fileName);

	/// <summary>
	/// Gets the reprojection error of the camera calibration.
	/// </summary>
	/// <returns>The reprojection error if the camera is calibrated, NaN otherwise</returns>
	double GetReprojectionError();

private:
	Camera* m_pCamera;
};

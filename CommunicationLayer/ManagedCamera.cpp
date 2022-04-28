// Author:	Liam Scholte
// Created:	3/6/2022 1:23:39 PM
// This file contains the implementations for ManagedCamera
// See ManagedCamera.h for documentation

#include "ManagedCamera.h"

#pragma managed(push, off)
#include <Chess\ArView\Camera.h>
#pragma managed(pop)

#include <msclr\marshal_cppstd.h>

#using <WindowsBase.dll>

ManagedCamera::ManagedCamera()
	: m_pCamera(new Chess::ArView::Camera())
{
}

ManagedCamera::~ManagedCamera()
{
	delete m_pCamera;
}

System::Windows::Media::Imaging::BitmapSource^ ManagedCamera::GetImage(bool showCalibrationInfo, bool enableHandThresholding)
{
	//Copy the image data into a managed array which can be garbage collected
	std::vector<unsigned char> data = m_pCamera->getImage(showCalibrationInfo, enableHandThresholding);
	auto managedData = gcnew array<byte>(data.size());
	pin_ptr<byte> managedDataStart = &managedData[0];
	std::memcpy(managedDataStart, data.data(), data.size());

	return System::Windows::Media::Imaging::BitmapSource::Create(
		m_pCamera->getWidth(),
		m_pCamera->getHeight(),
		96,
		96,
		System::Windows::Media::PixelFormats::Bgr24,
		nullptr,
		managedData,
		3 * m_pCamera->getWidth());
}

bool ManagedCamera::CanSaveCalibrationImage()
{
	return m_pCamera->canSaveCalibrationImage();
}

bool ManagedCamera::SaveCalibrationImage()
{
	return m_pCamera->saveCalibrationImage();
}

int ManagedCamera::GetCalibrationImageCount()
{
	return m_pCamera->getCalibrationImageCount();
}

bool ManagedCamera::CanCalibrateCamera()
{
	return m_pCamera->canCalibrate();
}

bool ManagedCamera::CalibrateCamera()
{
	return m_pCamera->calibrate();
}

void ManagedCamera::ResetCalibration()
{
	return m_pCamera->resetCalibration();
}

bool ManagedCamera::LoadCalibrationFromFile(System::String^ fileName)
{
	std::string unmangedFileName = msclr::interop::marshal_as<std::string>(fileName);
	return m_pCamera->loadCalibrationFromFile(unmangedFileName);
}

bool ManagedCamera::SaveCalibrationToFile(System::String^ fileName)
{
	std::string unmangedFileName = msclr::interop::marshal_as<std::string>(fileName);
	return m_pCamera->saveCalibrationToFile(unmangedFileName);
}

double ManagedCamera::GetReprojectionError()
{
	return m_pCamera->getReprojectionError();
}

void ManagedCamera::HandleLeftClick(float x, float y)
{
	m_pCamera->handleLeftClick(x, y);
}

void ManagedCamera::HandleRightClick()
{
	m_pCamera->handleRightClick();
}
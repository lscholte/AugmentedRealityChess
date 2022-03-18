// Author:	Liam Scholte
// Created:	3/12/2022 9:52:46 AM
// This file contains the class definition for ObjectLoader

#pragma once

#include "DrawableObject.h"

#include <string>
#include <vector>
#include <memory>

/// <summary>
/// A loader for various graphical file formats into
/// representations suitable for rendering.
/// </summary>
class ObjectLoader
{
public:
	virtual ~ObjectLoader();

	/// <summary>
	/// Reads a file into a vector of DrawableObjects.
	/// </summary>
	/// <param name="filePath">The path to a file to read</param>
	/// <param name="meshes">The resulting vector of DrawableObjects</param>
	/// <returns></returns>
	bool load(
		std::string const& filePath,
		std::vector<std::shared_ptr<DrawableObject>>& teapotMeshes);
};

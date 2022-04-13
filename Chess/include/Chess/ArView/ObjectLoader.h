// Author:	Liam Scholte
// Created:	3/12/2022 9:52:46 AM
// This file contains the class definition for ObjectLoader

#pragma once

#include <glm/fwd.hpp>

#include <string>
#include <memory>

namespace Chess
{
namespace ArView
{
	class DrawableObject;

	/// <summary>
	/// A loader for various graphical file formats into
	/// representations suitable for rendering.
	/// </summary>
	class ObjectLoader
	{
	public:
		virtual ~ObjectLoader();

		/// <summary>
		/// Reads a file into a DrawableObject.
		/// </summary>
		/// <param name="filePath">The path to a file to read</param>
		/// <param name="color">The color to assign the vertices</param>
		/// <returns>A pointer to a DrawableObject if loaded successfully, empty pointer otherwise</returns>
		std::shared_ptr<DrawableObject> load(
			std::string const& filePath,
			glm::vec3 const& color);
	};
}
}

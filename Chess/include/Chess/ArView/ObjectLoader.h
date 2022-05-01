// Author:	Liam Scholte
// Created:	3/12/2022 9:52:46 AM
// This file contains the class definition for ObjectLoader

#pragma once

#include <glm/fwd.hpp>

#include <string>
#include <memory>
#include <filesystem>

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
		ObjectLoader();
		virtual ~ObjectLoader();

		/// <summary>
		/// Reads a file into a DrawableObject.
		/// </summary>
		/// <param name="filePath">The path to a file to read</param>
		/// <returns>A pointer to a DrawableObject if loaded successfully, empty pointer otherwise</returns>
		std::shared_ptr<DrawableObject> load(std::filesystem::path const& filePath);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
}

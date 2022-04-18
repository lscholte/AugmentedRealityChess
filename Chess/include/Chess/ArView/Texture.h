// Author:	Liam Scholte
// Created:	4/11/2022 11:06:27 PM
// This file contains the class definition for Texture

#pragma once

#include <string>
#include <filesystem>

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// Represents a texture that can be rendered onto objects.
	/// </summary>
	struct Texture
	{
		/// <summary>
		/// The ID for the OpenGL texture
		/// </summary>
		unsigned int id;

		/// <summary>
		/// The type of texture (e.g. diffuse, specular, etc)
		/// </summary>
		std::string type;

		/// <summary>
		/// The path to the texture
		/// </summary>
		std::filesystem::path path;
	};
}
}
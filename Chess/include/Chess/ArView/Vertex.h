// Author:	Liam Scholte
// Created:	3/13/2022 7:14:24 PM
// This file contains the class definition for Vertex

#pragma once

#include <glm/ext.hpp>

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// Represents a Vertex that can be sent to a shader in OpenGL
	/// </summary>
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 textureCoord;
	};

}
}

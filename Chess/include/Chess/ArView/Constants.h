// Author:	Liam Scholte
// Created:	3/13/2022 11:02:53 PM
// This is a collection of constants related to OpenGL.

#pragma once

namespace Chess
{
namespace ArView
{
	unsigned int constexpr GLSL_POSITION_LOCATION = 0;
	unsigned int constexpr GLSL_COLOR_LOCATION = 1;
	unsigned int constexpr GLSL_NORMAL_LOCATION = 2;
	unsigned int constexpr GLSL_TEXTURE_COORD_LOCATION = 3;

	float constexpr Z_NEAR = 1.0f;
	float constexpr Z_FAR = 1000.0f;
}
}

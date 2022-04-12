// Author:	Liam Scholte
// Created:	4/11/2022 11:06:27 PM
// This file contains the class definition for Texture

#pragma once

#include <string>

namespace Chess
{
namespace ArView
{
	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};
}
}
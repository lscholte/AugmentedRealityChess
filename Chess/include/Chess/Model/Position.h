// Author:	Liam Scholte
// Created:	3/26/2022 10:03:47 PM
// This file contains the class definition for Position

#pragma once

#include <Chess/Macros.h>

namespace Chess
{
namespace Model
{
	struct EXPORT Position
	{
		unsigned char rank;
		unsigned char file;

		Position(unsigned char rank, unsigned char file);

		bool operator==(Position other) const;
		bool operator!=(Position other) const;
	};
}
}

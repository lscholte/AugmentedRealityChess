// Author:	Liam Scholte
// Created:	3/26/2022 10:03:47 PM
// This file contains the implementations for Position
// See Position.h for documentation

#pragma once

#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	Position::Position(unsigned char rank, unsigned char file)
		: rank(rank)
		, file(file)
	{}

	bool Position::operator==(Position other) const
	{
		return rank == other.rank && file == other.file;
	}

	bool Position::operator!=(Position other) const
	{
		return !(*this == other);
	}
}
}

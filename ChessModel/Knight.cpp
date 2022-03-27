// Author:	Liam Scholte
// Created:	3/26/2022 11:48:18 PM
// This file contains the implementations for Knight
// See Knight.h for documentation

#include "Knight.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Knight::Knight(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Knight::~Knight() = default;

	PieceType Knight::getType() const
	{
		return PieceType::Knight;
	}
}
}
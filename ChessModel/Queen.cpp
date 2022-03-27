// Author:	Liam Scholte
// Created:	3/26/2022 11:48:37 PM
// This file contains the implementations for Queen
// See Queen.h for documentation

#include "Queen.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Queen::Queen(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Queen::~Queen() = default;

	PieceType Queen::getType() const
	{
		return PieceType::Queen;
	}
}
}
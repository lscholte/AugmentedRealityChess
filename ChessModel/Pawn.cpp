// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the implementations for Pawn
// See Pawn.h for documentation

#include "Pawn.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Pawn::Pawn(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Pawn::~Pawn() = default;

	PieceType Pawn::getType() const
	{
		return PieceType::Pawn;
	}
}
}
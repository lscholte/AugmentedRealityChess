// Author:	Liam Scholte
// Created:	3/26/2022 11:48:07 PM
// This file contains the implementations for Rook
// See Rook.h for documentation

#include "Rook.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Rook::Rook(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Rook::~Rook() = default;

	PieceType Rook::getType() const
	{
		return PieceType::Rook;
	}
}
}
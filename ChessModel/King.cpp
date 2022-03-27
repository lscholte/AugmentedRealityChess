// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the implementations for King
// See King.h for documentation

#include "King.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	King::King(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	King::~King() = default;

	PieceType King::getType() const
	{
		return PieceType::King;
	}
}
}
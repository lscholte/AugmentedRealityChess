// Author:	Liam Scholte
// Created:	3/26/2022 11:48:07 PM
// This file contains the implementations for Rook
// See Rook.h for documentation

#include "Rook.h"
#include "Board.h"
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

	std::vector<Position> Rook::getLegalMoves(Board const& board) const
	{
		//TODO: Implement this
		//Can move horizontally or vertically
		return {};
	}
}
}
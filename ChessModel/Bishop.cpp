// Author:	Liam Scholte
// Created:	3/26/2022 11:48:28 PM
// This file contains the implementations for Bishop
// See Bishop.h for documentation

#include "Bishop.h"
#include "Board.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Bishop::Bishop(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Bishop::~Bishop() = default;

	PieceType Bishop::getType() const
	{
		return PieceType::Bishop;
	}

	std::vector<Position> Bishop::getLegalMoves(Board const& board) const
	{
		//TODO: Implement this
		//Can move diagonally only
		return {};
	}
}
}

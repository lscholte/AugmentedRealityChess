// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the implementations for Pawn
// See Pawn.h for documentation

#include "Pawn.h"
#include "Board.h"
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

	std::vector<Position> Pawn::getLegalMoves(Board const& board) const
	{
		//TODO: Implement this
		//Can move 1 square forward.
		//Can move 2 squares forward if first move
		//Can move diagonally 1 square to capture
		//There is also en passant move (ignore this for now)
		return {};
	}
}
}
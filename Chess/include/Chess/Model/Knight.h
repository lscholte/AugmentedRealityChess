// Author:	Liam Scholte
// Created:	3/26/2022 11:48:18 PM
// This file contains the class definition for Knight

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	class Knight
		: public Piece
	{
	public:
		Knight(bool isWhite, Position position);
		virtual ~Knight();

		virtual PieceType getType() const override;

		virtual std::vector<Position> getLegalMoves(Board const& board) const override;

	};
}
}
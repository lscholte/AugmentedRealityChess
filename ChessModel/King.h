// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the class definition for King

#pragma once

#include "Piece.h"

namespace Chess
{
namespace Model
{
	class King
		: public Piece
	{
	public:
		King(bool isWhite, Position position);
		virtual ~King();

		virtual PieceType getType() const override;

		virtual std::vector<Position> getLegalMoves(Board const& board) const override;

	};
}
}
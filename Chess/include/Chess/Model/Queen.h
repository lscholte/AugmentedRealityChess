// Author:	Liam Scholte
// Created:	3/26/2022 11:48:37 PM
// This file contains the class definition for Queen

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	class Queen
		: public Piece
	{
	public:
		Queen(bool isWhite, Position position);
		Queen(Queen const& otherQueen);
		virtual ~Queen();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}

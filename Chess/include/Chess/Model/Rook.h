// Author:	Liam Scholte
// Created:	3/26/2022 11:48:07 PM
// This file contains the class definition for Rook

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	class Rook
		: public Piece
	{
	public:
		Rook(bool isWhite, Position position);
		Rook(Rook const& otherRook);
		virtual ~Rook();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
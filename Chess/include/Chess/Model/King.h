// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the class definition for King

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	class King
		: public Piece
	{
	public:
		King(bool isWhite, Position position);
		King(King const& otherKing);
		virtual ~King();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
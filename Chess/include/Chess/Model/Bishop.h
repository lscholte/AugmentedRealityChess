// Author:	Liam Scholte
// Created:	3/26/2022 11:48:28 PM
// This file contains the class definition for Bishop

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	class Bishop
		: public Piece
	{
	public:
		Bishop(bool isWhite, Position position);
		virtual ~Bishop();

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
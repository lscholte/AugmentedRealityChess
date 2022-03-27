// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the class definition for Pawn

#pragma once

#include "Piece.h"

namespace Chess
{
namespace Model
{
	class Pawn
		: public Piece
	{
	public:
		Pawn(bool isWhite, Position position);
		virtual ~Pawn();

		virtual PieceType getType() const override;

	};
}
}

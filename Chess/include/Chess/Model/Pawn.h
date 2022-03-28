// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the class definition for Pawn

#pragma once

#include <Chess/Model/Piece.h>

#include <memory>

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

		virtual std::vector<Position> getLegalMoves(Board const& board) const override;

		virtual bool move(Board const& board, Position newPosition) override;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;

	};
}
}

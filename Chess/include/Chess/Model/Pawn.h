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
	/// <summary>
	/// A pawn chess piece that can be placed on a board.
	/// </summary>
	class Pawn
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a pawn in a position.
		/// </summary>
		/// <param name="isWhite">Whether the pawn is white or black</param>
		/// <param name="position">The position of the pawn</param>
		Pawn(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another pawn.
		/// </summary>
		/// <param name="otherPiece">The other pawn to copy</param>
		Pawn(Pawn const& otherPawn);

		virtual ~Pawn();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;

		virtual bool move(Board& board, Position newPosition) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}
}

// Author:	Liam Scholte
// Created:	3/26/2022 11:48:37 PM
// This file contains the class definition for Queen

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A queen chess piece that can be placed on a board.
	/// </summary>
	class Queen
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a queen in a position.
		/// </summary>
		/// <param name="isWhite">Whether the queen is white or black</param>
		/// <param name="position">The position of the queen</param>
		Queen(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another queen.
		/// </summary>
		/// <param name="otherPiece">The other queen to copy</param>
		Queen(Queen const& otherQueen);

		virtual ~Queen();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}

// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the class definition for King

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A king chess piece that can be placed on a board.
	/// </summary>
	class King
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a king in a position.
		/// </summary>
		/// <param name="isWhite">Whether the king is white or black</param>
		/// <param name="position">The position of the king</param>
		King(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another king.
		/// </summary>
		/// <param name="otherPiece">The other king to copy</param>
		King(King const& otherKing);

		virtual ~King();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
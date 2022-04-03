// Author:	Liam Scholte
// Created:	3/26/2022 11:48:18 PM
// This file contains the class definition for Knight

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A knight chess piece that can be placed on a board.
	/// </summary>
	class Knight
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a knight in a position.
		/// </summary>
		/// <param name="isWhite">Whether the knight is white or black</param>
		/// <param name="position">The position of the knight</param>
		Knight(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another knight.
		/// </summary>
		/// <param name="otherPiece">The other knight to copy</param>
		Knight(Knight const& otherKnight);

		virtual ~Knight();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
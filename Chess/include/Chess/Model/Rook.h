// Author:	Liam Scholte
// Created:	3/26/2022 11:48:07 PM
// This file contains the class definition for Rook

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A rook chess piece that can be placed on a board.
	/// </summary>
	class Rook
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a rook in a position.
		/// </summary>
		/// <param name="isWhite">Whether the rook is white or black</param>
		/// <param name="position">The position of the rook</param>
		Rook(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another rook.
		/// </summary>
		/// <param name="otherPiece">The other rook to copy</param>
		Rook(Rook const& otherRook);

		virtual ~Rook();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
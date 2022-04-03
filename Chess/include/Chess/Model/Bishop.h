// Author:	Liam Scholte
// Created:	3/26/2022 11:48:28 PM
// This file contains the class definition for Bishop

#pragma once

#include <Chess/Model/Piece.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A bishop chess piece that can be placed on a board.
	/// </summary>
	class Bishop
		: public Piece
	{
	public:
		/// <summary>
		/// Constructs a bishop in a position.
		/// </summary>
		/// <param name="isWhite">Whether the bishop is white or black</param>
		/// <param name="position">The position of the bishop</param>
		Bishop(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another bishop.
		/// </summary>
		/// <param name="otherPiece">The other bishop to copy</param>
		Bishop(Bishop const& otherBishop);

		virtual ~Bishop();

		virtual std::shared_ptr<Piece> clone() const override;

		virtual PieceType getType() const override;

		virtual std::vector<Position> getAttackingPositions(Board const& board) const override;
	};
}
}
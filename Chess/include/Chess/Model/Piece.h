// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the class definition for Piece

#pragma once

#include <Chess/Macros.h>

#include <vector>
#include <memory>

namespace Chess
{
namespace Model
{
	class Board;
	struct Position;

	enum class PieceType
	{
		Pawn, Rook, Knight, Bishop, Queen, King
	};

	class EXPORT Piece
	{
	public:
		Piece(bool isWhite, Position position);
		virtual ~Piece();

		virtual PieceType getType() const = 0;

		virtual bool isWhite() const;
		virtual Position getPosition() const;

		virtual bool move(Board const& board, Position newPosition);

		/// <summary>
		/// Gets the positions that a piece may legally move to.
		/// A legal position will not include the current position.
		/// </summary>
		/// <param name="board">The board containing all other pieces</param>
		/// <returns>A vector of legal positions this piece can move to in no particular order</returns>
		virtual std::vector<Position> getLegalMoves(Board const& board) const = 0;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}

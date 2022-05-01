// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the class definition for Piece

#pragma once

#include <Chess/Macros.h>
#include <Chess/Model/FwdDecl.h>

#include <vector>
#include <memory>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// Represents possible types of chess pieces.
	/// </summary>
	enum class PieceType
	{
		Pawn, Rook, Knight, Bishop, Queen, King
	};

	/// <summary>
	/// An abstract chess piece that can be placed on a board.
	/// </summary>
	class EXPORT Piece : public std::enable_shared_from_this<Piece>
	{
	public:
		/// <summary>
		/// Constructs a piece in a position.
		/// </summary>
		/// <param name="isWhite">Whether the piece is white or black</param>
		/// <param name="position">The position of the piece</param>
		Piece(bool isWhite, Position position);

		/// <summary>
		/// Constructs a deep copy of another piece.
		/// </summary>
		/// <param name="otherPiece">The other piece to copy</param>
		Piece(Piece const& otherPiece);

		virtual ~Piece();

		/// <summary>
		/// Performs a deep copy of this piece.
		/// </summary>
		/// <returns>A shared pointer to a deep copy of this piece</returns>
		virtual std::shared_ptr<Piece> clone() const = 0;

		/// <summary>
		/// Gets the type of this piece.
		/// </summary>
		/// <returns>The type of this piece</returns>
		virtual PieceType getType() const = 0;

		/// <summary>
		/// Determines if this piece is white or black.
		/// </summary>
		/// <returns>True if this piece is white, false if black</returns>
		virtual bool isWhite() const;

		/// <summary>
		/// Gets the current position of this piece.
		/// </summary>
		/// <returns>The current position of this piece</returns>
		virtual Position getPosition() const;

		/// <summary>
		/// Moves this piece to a new position on the board.
		/// Does nothing if the move would be illegal.
		/// </summary>
		/// <param name="board">The board containing all other pieces</param>
		/// <param name="newPosition">The position to move to</param>
		/// <returns>True if the move was completed, false otherwise</returns>
		virtual bool move(Board& board, Position newPosition);

		/// <summary>
		/// Gets the positions that this piece threatens.
		/// A position that is under attack is not necessarily a legal move.
		/// </summary>
		/// <param name="board">The board containing all other pieces</param>
		/// <returns>A vector of positions that this piece threatens in no particular order</returns>
		virtual std::vector<Position> getAttackingPositions(Board const& board) const = 0;

		/// <summary>
		/// Gets the positions that a piece may legally move to.
		/// A legal position will not include the current position.
		/// </summary>
		/// <param name="board">The board containing all other pieces</param>
		/// <returns>A vector of legal positions this piece can move to in no particular order</returns>
		virtual std::vector<Position> getLegalMoves(Board const& board) const;

		/// <summary>
		/// Determines if there are any opposing pieces on the board
		/// that are attacking this piece's current position.
		/// </summary>
		/// <param name="board">The board containing all other pieces</param>
		/// <returns>True if this piece is under attack by an opposing piece, false otherwise</returns>
		virtual bool isUnderAttack(Board const& board) const;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
}

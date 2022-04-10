// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the class definition for Board

#pragma once

#include <Chess/Macros.h>
#include <Chess/Model/FwdDecl.h>

#include <unordered_set>
#include <memory>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// Represents an 8x8 chessboard that can hold various chess pieces.
	/// </summary>
	class EXPORT Board
	{
	public:

		/// <summary>
		/// Constructs a board with pieces in their appropriate
		/// starting positions for white and black
		/// </summary>
		Board();

		/// <summary>
		/// Constructs a board from a deep copy of an existing board
		/// </summary>
		/// <param name="otherBoard">The existing board to deep copy</param>
		Board(Board const& otherBoard);

		virtual ~Board();

		/// <summary>
		/// Gets the dimensions of the board.
		/// </summary>
		/// <returns>The dimensions of the board</returns>
		Size getSize() const;

		/// <summary>
		/// Gets all of the pieces on the board.
		/// </summary>
		/// <returns>All of the pieces on the board</returns>
		std::unordered_set<std::shared_ptr<Piece>> const& getPieces() const;

		/// <summary>
		/// Gets all of the pieces of the specified color on the board.
		/// </summary>
		/// <param name="isWhite">Whether to get white or black pieces</param>
		/// <returns>All of the pieces of the specified color on the board</returns>
		std::unordered_set<std::shared_ptr<Piece>> const& getPieces(bool isWhite) const;

		/// <summary>
		/// Gets the piece at the specified position.
		/// </summary>
		/// <param name="position">The position to search for a piece</param>
		/// <returns>The piece at the position or nullptr if no piece exists</returns>
		std::shared_ptr<Piece> getPiece(Position position) const;

		/// <summary>
		/// Removes the specified piece from the board.
		/// </summary>
		/// <param name="pPiece">The piece to remove</param>
		/// <returns>True if the piece was removed, false otherwise</returns>
		bool removePiece(std::shared_ptr<Piece> pPiece);

		/// <summary>
		/// Promotes the specified piece to a piece of a particular type.
		/// If the piece can be promoted, it will be removed from the board and
		/// replaced with a new piece of the specified promotion type.
		/// </summary>
		/// <param name="pPieceToPromote">The piece to promote</param>
		/// <param name="promotionType">The type of piece to promote to</param>
		/// <returns>The newly promoted piece or an empty pointer if promotion did not occur</returns>
		std::shared_ptr<Piece> promotePiece(std::shared_ptr<Piece> pPieceToPromote, PieceType promotionType);

		/// <summary>
		/// Determines if a specified position is on the board.
		/// </summary>
		/// <param name="position">The position to check</param>
		/// <returns>True if the position is on the board, false otherwise</returns>
		bool isPositionOnBoard(Position position) const;

		/// <summary>
		/// Determines if it is possible to move a piece of a particular color to a particular position.
		/// A position is considered possible if it is on the board and there is no piece of the
		/// same color in the position already.
		/// </summary>
		/// <param name="isWhite">Whether the piece that is being checked is white or black</param>
		/// <param name="position">The position to check</param>
		/// <returns>True if a piece could possibly be moved into the specified position, false otherwise</returns>
		bool isPositionPossible(bool isWhite, Position position) const;

		/// <summary>
		/// Determines if the king for a particular color is in check given the current state of the board.
		/// </summary>
		/// <param name="isWhite">Whether the king that is being checked is white or black</param>
		/// <returns>True if the king is in check, false otherwise</returns>
		bool isKingInCheck(bool isWhite) const;


	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}


// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the class definition for Board

#pragma once

#include <Chess/Macros.h>

#include <unordered_set>
#include <memory>

namespace Chess
{
namespace Model
{
	class Piece;
	struct Position;
	struct Size;

	class EXPORT Board
	{
	public:
		Board();
		virtual ~Board();

		Size getSize() const;

		std::unordered_set<std::shared_ptr<Piece>> const& getPieces() const;
		std::unordered_set<std::shared_ptr<Piece>> const& getPieces(bool isWhite) const;

		/// <summary>
		/// Gets the piece at the specified position.
		/// </summary>
		/// <param name="position">The position to search for a piece</param>
		/// <returns>The piece at the position or nullptr if no piece exists</returns>
		std::shared_ptr<Piece> getPiece(Position position) const;

		bool removePiece(std::shared_ptr<Piece> pPiece);

		bool isPositionOnBoard(Position position) const;
		bool isPositionPossible(bool isWhite, Position position) const;
		bool isPositionLegal(bool isWhite, Position position) const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}


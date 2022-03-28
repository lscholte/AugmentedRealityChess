// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the class definition for Board

#pragma once

#include <Chess/Model/Macros.h>

#include <vector>
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

		std::vector<std::shared_ptr<Piece>> const& getPieces() const;

		/// <summary>
		/// Gets the piece at the specified position.
		/// </summary>
		/// <param name="position">The position to search for a piece</param>
		/// <returns>The piece at the position or nullptr if no piece exists</returns>
		std::shared_ptr<Piece> getPiece(Position position) const;

		bool isPositionOnBoard(Position position) const;
		bool isPositionLegal(bool isWhite, Position position) const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}


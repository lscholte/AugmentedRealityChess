// Author:	Liam Scholte
// Created:	4/9/2022 12:37:31 PM
// This file contains the class definition for PieceFactory

#pragma once

#include <Chess/Model/FwdDecl.h>

#include <memory>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A factory class that creates chess pieces.
	/// </summary>
	class PieceFactory
	{
	public:

		/// <summary>
		/// Constructs a factory for creating pieces.
		/// </summary>
		/// <param name="isWhite">Whether this factory creates white or black pieces</param>
		PieceFactory(bool isWhite);
		virtual ~PieceFactory();

		/// <summary>
		/// Creates a chess piece of a given type at a position on the board
		/// </summary>
		/// <param name="type">The type of piece to create</param>
		/// <param name="position">The position of the piece on the board</param>
		/// <returns>The created chess piece</returns>
		std::shared_ptr<Piece> create(PieceType type, Position position) const;

	private:
		bool m_isWhite;
	};

}
}

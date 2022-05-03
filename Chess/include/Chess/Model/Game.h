// Author:	Liam Scholte
// Created:	3/26/2022 9:50:09 PM
// This file contains the class definition for Game

#pragma once

#include <Chess/Macros.h>
#include <Chess/Model/FwdDecl.h>

#include <memory>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// A game of chess that maintains the overall state of the game.
	/// </summary>
	class EXPORT Game
	{
	public:
		/// <summary>
		/// Constructs a game of chess.
		/// </summary>
		Game();

		virtual ~Game();

		/// <summary>
		/// Determines if it is white or black to move next.
		/// </summary>
		/// <returns>True if white is to move next, false if black</returns>
		bool isWhiteMove() const;

		/// <summary>
		/// Gets the chessboard, which also contains the pieces in the game.
		/// </summary>
		/// <returns>The chessboard for this game of chess</returns>
		Board const& getBoard() const;

		/// <summary>
		/// Attempts to move the piece from one position to a new position.
		/// Movement may fail for reasons such as
		/// <list type="bullet">
		///		<item>there is no available piece at the chosen initial position</item>
		///		<item>the piece at the chosen initial position is not the correct color</item>
		///		<item>the new position is not within the set of legal moves for the chosen piece</item>
		/// </list>
		/// </summary>
		/// <param name="currentPosition">The initial position</param>
		/// <param name="newPosition">The destination position</param>
		/// <returns>True if the move was successfully completed, false otherwise</returns>
		bool move(Position currentPosition, Position newPosition);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}
}

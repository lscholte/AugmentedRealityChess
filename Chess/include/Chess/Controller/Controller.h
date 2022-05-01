// Author:	Liam Scholte
// Created:	3/27/2022 4:20:46 PM
// This file contains the class definition for Controller

#pragma once

#include <Chess/Macros.h>
#include <Chess/Model/FwdDecl.h>

#include <memory>

namespace Chess
{
namespace Controller
{
	/// <summary>
	/// Handles inputs to a game of chess
	/// </summary>
	class EXPORT Controller
	{
	public:
		/// <summary>
		/// Constructs a controller for a game of chess.
		/// </summary>
		Controller();

		virtual ~Controller();

		/// <summary>
		/// Gets the game of chess
		/// </summary>
		/// <returns>The game of chess</returns>
		Model::Game const& getGame() const;

		/// <summary>
		/// Gets the piece at the selected position.
		/// </summary>
		/// <returns>
		/// The piece at the selected position or an empty pointer
		/// if no piece exists at that position or there is no selected position
		/// </returns>
		std::shared_ptr<Model::Piece> getSelectedPiece() const;

		/// <summary>
		/// Selects the specified position. This may result in a piece
		/// being moved if the selected position is a legal move for
		/// a piece at an already selected position.
		/// </summary>
		/// <param name="position">The position to select</param>
		void selectPosition(Model::Position position);

		/// <summary>
		/// Unselects a position.
		/// </summary>
		void unselectPosition();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}
}

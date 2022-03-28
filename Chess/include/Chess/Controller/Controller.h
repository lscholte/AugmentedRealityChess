// Author:	Liam Scholte
// Created:	3/27/2022 4:20:46 PM
// This file contains the class definition for Controller

#pragma once

#include <Chess/Macros.h>

#include <memory>

namespace Chess
{
namespace Model
{
	class Game;
	class Piece;
	struct Position;
}

namespace Controller
{
	class EXPORT Controller
	{
	public:
		Controller();
		virtual ~Controller();

		Model::Game const& getGame() const;

		std::shared_ptr<Model::Piece> getSelectedPiece() const;

		void selectPosition(Model::Position position);
		void unselectPosition();

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;

	};
}
}

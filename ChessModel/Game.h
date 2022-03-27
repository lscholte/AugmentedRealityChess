// Author:	Liam Scholte
// Created:	3/26/2022 9:50:09 PM
// This file contains the class definition for Game

#pragma once

#include "Macros.h"

#include <memory>

namespace Chess
{
namespace Model
{
	class Board;
	struct Position;

	class EXPORT Game
	{
	public:
		Game();
		virtual ~Game();

		bool isWhiteMove() const;

		Board const& getBoard() const;

		bool move(Position currentPosition, Position newPosition);

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;

	};
}
}

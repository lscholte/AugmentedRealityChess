// Author:	Liam Scholte
// Created:	3/26/2022 9:50:09 PM
// This file contains the class definition for Game

#pragma once

#include <memory>

namespace Chess
{
namespace Model
{
	class Board;

	class Game
	{
	public:
		Game();
		virtual ~Game();

		bool isWhiteMove() const;

		Board const& getBoard() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;

	};
}
}

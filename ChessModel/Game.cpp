// Author:	Liam Scholte
// Created:	3/26/2022 9:50:09 PM
// This file contains the implementations for Game
// See Game.h for documentation

#include "Game.h"
#include "Board.h"

namespace Chess
{
namespace Model
{

	struct Game::Impl
	{
		bool isWhiteMove;
		Board board;

		Impl()
			: isWhiteMove(true)
		{}
	};

	Game::Game()
		: m_pImpl(std::make_shared<Impl>())
	{}

	Game::~Game() = default;

	bool Game::isWhiteMove() const
	{
		return m_pImpl->isWhiteMove;
	}

	Board const& Game::getBoard() const
	{
		return m_pImpl->board;
	}
}
}

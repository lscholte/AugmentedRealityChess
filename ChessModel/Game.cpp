// Author:	Liam Scholte
// Created:	3/26/2022 9:50:09 PM
// This file contains the implementations for Game
// See Game.h for documentation

#include "Game.h"
#include "Board.h"
#include "Position.h"
#include "Piece.h"

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
	{
	}

	Game::~Game() = default;

	bool Game::isWhiteMove() const
	{
		return m_pImpl->isWhiteMove;
	}

	Board const& Game::getBoard() const
	{
		return m_pImpl->board;
	}

	bool Game::move(Position currentPosition, Position newPosition)
	{
		std::shared_ptr<Piece> pPiece = m_pImpl->board.getPiece(currentPosition);
		if (!pPiece)
		{
			//No piece exists at current position
			return false;
		}

		if (pPiece->isWhite() != isWhiteMove())
		{
			//Selected piece is not the correct color for moving
			return false;
		}

		std::vector<Position> legalPositions = pPiece->getLegalMoves(getBoard());
		auto legalPositionIter = std::find_if(
			legalPositions.cbegin(),
			legalPositions.cend(),
			[newPosition](Position position)
			{
				return position == newPosition;
			});
		if (legalPositionIter == legalPositions.cend())
		{
			//Position is not legal for the selected piece
			return false;
		}

		pPiece->move(getBoard(), newPosition);
		m_pImpl->isWhiteMove = !m_pImpl->isWhiteMove;

		return true;
	}
}
}

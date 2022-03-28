// Author:	Liam Scholte
// Created:	3/27/2022 4:20:46 PM
// This file contains the implementations for Controller
// See Controller.h for documentation

#include <Chess/Controller/Controller.h>

#include <Chess/Model/Piece.h>
#include <Chess/Model/Game.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Board.h>

namespace Chess
{
namespace Controller
{
	struct Controller::Impl
	{
		Model::Game game;
		std::shared_ptr<Model::Piece> pSelectedPiece;
	};

	Controller::Controller()
		: m_pImpl(std::make_shared<Impl>())
	{}

	Controller::~Controller() = default;

	Model::Game const& Controller::getGame() const
	{
		return m_pImpl->game;
	}

	std::shared_ptr<Model::Piece> Controller::getSelectedPiece() const
	{
		return m_pImpl->pSelectedPiece;
	}

	void Controller::selectPosition(Model::Position position)
	{

		if (!m_pImpl->pSelectedPiece)
		{
			std::shared_ptr<Model::Piece> pPiece = m_pImpl->game.getBoard().getPiece(position);
			if (pPiece && m_pImpl->game.isWhiteMove() == pPiece->isWhite())
			{
				m_pImpl->pSelectedPiece = pPiece;
				return;
			}
		}
		else
		{
			//Move the piece
			bool moveSuccessful = m_pImpl->game.move(m_pImpl->pSelectedPiece->getPosition(), position);
			if (moveSuccessful)
			{
				m_pImpl->pSelectedPiece = nullptr;
				return;
			}
		}
	}

	void Controller::unselectPosition()
	{
		m_pImpl->pSelectedPiece = nullptr;
	}
}
}

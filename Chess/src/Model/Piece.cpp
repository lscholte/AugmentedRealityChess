// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the implementations for Piece
// See Piece.h for documentation

#include <Chess/Model/Piece.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Board.h>

namespace Chess
{
namespace Model
{
	struct Piece::Impl
	{
		bool isWhite;
		Position position;

		Impl(bool isWhite, Position position)
			: isWhite(isWhite)
			, position(position)
		{}

		void moveWithoutChecking(Board& board, Position newPosition)
		{
			std::shared_ptr<Piece> pExistingPiece = board.getPiece(newPosition);
			if (pExistingPiece)
			{
				board.removePiece(pExistingPiece);
			}

			position = newPosition;
		}
	};

	Piece::Piece(bool isWhite, Position position)
		: m_pImpl(std::make_unique<Impl>(isWhite, position))
	{}

	Piece::Piece(Piece const& otherPiece)
		: Piece(otherPiece.isWhite(), otherPiece.getPosition())
	{}

	Piece::~Piece() = default;

	bool Piece::isWhite() const
	{
		return m_pImpl->isWhite;
	}

	Position Piece::getPosition() const
	{
		return m_pImpl->position;
	}

	bool Piece::move(Board& board, Position newPosition)
	{
		//TODO: Calculations would have likely already been previously calculated.
		//I can probably eliminate this repeated calculation with some clever caching
		std::vector<Position> legalPositions = getLegalMoves(board);
		auto legalPositionIter = std::find_if(
			legalPositions.cbegin(),
			legalPositions.cend(),
			[newPosition](Position position)
			{
				return position == newPosition;
			});
		if (legalPositionIter == legalPositions.cend())
		{
			//Position is not legal for this piece
			return false;
		}

		m_pImpl->moveWithoutChecking(board, newPosition);
		return true;
	}

	bool Piece::isUnderAttack(Board const& board) const
	{
		for (std::shared_ptr<Piece> pOppositeColorPiece : board.getPieces(!isWhite()))
		{
			std::vector<Position> attackingPositions = pOppositeColorPiece->getAttackingPositions(board);
			auto iter = std::find_if(
				attackingPositions.cbegin(),
				attackingPositions.cend(),
				[currentPosition = getPosition()](Position position)
			{
				return position == currentPosition;
			});
			
			if (iter != attackingPositions.cend())
			{
				//This piece sits in a square that is threatened by an opposing piece
				return true;
			}
		}
		return false;
	}

	std::vector<Position> Piece::getLegalMoves(Board const& board) const
	{
		std::vector<Position> legalPositions;
		for (Position position : getAttackingPositions(board))
		{
			//On a separate copy of the board, try moving the piece
			//and then check if it puts the player's own King in check.
			//Use a copied board to avoid corrupting the state of the game.
			Board copiedBoard(board);
			std::shared_ptr<Piece> pCopiedPiece = copiedBoard.getPiece(getPosition());
			pCopiedPiece->m_pImpl->moveWithoutChecking(copiedBoard, position);
			if (!copiedBoard.isKingInCheck(isWhite()))
			{
				legalPositions.push_back(position);
			}
		}

		return legalPositions;
	}
}
}

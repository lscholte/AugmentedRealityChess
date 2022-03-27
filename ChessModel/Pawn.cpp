// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the implementations for Pawn
// See Pawn.h for documentation

#include "Pawn.h"
#include "Board.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	struct Pawn::Impl
	{
		bool isFirstMove;

		Impl()
			: isFirstMove(true)
		{}
	};

	Pawn::Pawn(bool isWhite, Position position)
		: Piece(isWhite, position)
		, m_pImpl(std::make_shared<Impl>())
	{}

	Pawn::~Pawn() = default;

	PieceType Pawn::getType() const
	{
		return PieceType::Pawn;
	}

	std::vector<Position> Pawn::getLegalMoves(Board const& board) const
	{
		std::vector<Position> legalPositions;

		Position currentPosition = getPosition();

		auto checkAndPushPosition = [isWhite = isWhite(), currentPosition, &legalPositions, &board](Position positionToCheck)
		{
			if (board.isPositionLegal(isWhite, positionToCheck))
			{
				legalPositions.push_back(positionToCheck);
			}
		};

		//Pawn can move 2 squares forward on first move
		if (m_pImpl->isFirstMove)
		{
			if (isWhite())
			{
				checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file));
			}
			else
			{
				checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file));
			}
		}

		//Otherwise pawn can move 1 square forward
		//or 1 square diagonally if an opposing piece is there for capture
		if (isWhite())
		{
			checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file));
			{
				Position positionToCheck(currentPosition.rank + 1, currentPosition.file + 1);
				std::shared_ptr<Piece> pPiece = board.getPiece(positionToCheck);
				if (pPiece)
				{
					checkAndPushPosition(positionToCheck);
				}
			}
			{
				Position positionToCheck(currentPosition.rank + 1, currentPosition.file - 1);
				std::shared_ptr<Piece> pPiece = board.getPiece(positionToCheck);
				if (pPiece)
				{
					checkAndPushPosition(positionToCheck);
				}
			}
		}
		else
		{
			checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file));
			{
				Position positionToCheck(currentPosition.rank - 1, currentPosition.file + 1);
				std::shared_ptr<Piece> pPiece = board.getPiece(positionToCheck);
				if (pPiece)
				{
					checkAndPushPosition(positionToCheck);
				}
			}
			{
				Position positionToCheck(currentPosition.rank - 1, currentPosition.file - 1);
				std::shared_ptr<Piece> pPiece = board.getPiece(positionToCheck);
				if (pPiece)
				{
					checkAndPushPosition(positionToCheck);
				}
			}
		}

		//TODO: There is also en passant move (ignore this for now)

		return legalPositions;
	}
}
}
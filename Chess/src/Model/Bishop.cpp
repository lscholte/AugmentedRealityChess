// Author:	Liam Scholte
// Created:	3/26/2022 11:48:28 PM
// This file contains the implementations for Bishop
// See Bishop.h for documentation

#include <Chess/Model/Bishop.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	Bishop::Bishop(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Bishop::Bishop(Bishop const& otherBishop)
		: Piece(otherBishop)
	{}

	Bishop::~Bishop() = default;

	std::shared_ptr<Piece> Bishop::clone() const
	{
		return std::make_shared<Bishop>(*this);
	}

	PieceType Bishop::getType() const
	{
		return PieceType::Bishop;
	}

	std::vector<Position> Bishop::getAttackingPositions(Board const& board) const
	{
		std::vector<Position> attackingPositions;

		Position currentPosition = getPosition();

		auto checkAndPushPosition = [isWhite = isWhite(), currentPosition, &attackingPositions, &board](Position positionToCheck)
		{
			if (board.isPositionPossible(isWhite, positionToCheck))
			{
				attackingPositions.push_back(positionToCheck);
			}
		};

		//Go up rank and up file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank += 1;
				positionToCheck.file += 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go down rank and up file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank -= 1;
				positionToCheck.file += 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go up rank and down file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank += 1;
				positionToCheck.file -= 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go down rank and down file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank -= 1;
				positionToCheck.file -= 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		return attackingPositions;
	}
}
}

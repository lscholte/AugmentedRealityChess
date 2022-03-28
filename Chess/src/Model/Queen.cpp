// Author:	Liam Scholte
// Created:	3/26/2022 11:48:37 PM
// This file contains the implementations for Queen
// See Queen.h for documentation

#include <Chess/Model/Queen.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	Queen::Queen(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Queen::~Queen() = default;

	PieceType Queen::getType() const
	{
		return PieceType::Queen;
	}

	std::vector<Position> Queen::getLegalMoves(Board const& board) const
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

		//Go up rank
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank += 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go down rank
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.rank -= 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go up file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.file += 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

		//Go down file
		{
			Position positionToCheck = currentPosition;
			do
			{
				positionToCheck.file -= 1;
				checkAndPushPosition(positionToCheck);
			} while (!board.getPiece(positionToCheck) && board.isPositionOnBoard(positionToCheck));
		}

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

		return legalPositions;
	}
}
}
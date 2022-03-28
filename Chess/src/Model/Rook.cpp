// Author:	Liam Scholte
// Created:	3/26/2022 11:48:07 PM
// This file contains the implementations for Rook
// See Rook.h for documentation

#include <Chess/Model/Rook.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	Rook::Rook(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Rook::~Rook() = default;

	PieceType Rook::getType() const
	{
		return PieceType::Rook;
	}

	std::vector<Position> Rook::getLegalMoves(Board const& board) const
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

		return legalPositions;
	}
}
}
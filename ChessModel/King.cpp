// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the implementations for King
// See King.h for documentation

#include "King.h"
#include "Board.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	King::King(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	King::~King() = default;

	PieceType King::getType() const
	{
		return PieceType::King;
	}

	std::vector<Position> King::getLegalMoves(Board const& board) const
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

		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 0));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank + 0, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 0));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 0, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file - 1));

		//TODO: Can also castle under certain circumstances (ignore this for now)

		return legalPositions;
	}
}
}
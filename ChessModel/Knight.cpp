// Author:	Liam Scholte
// Created:	3/26/2022 11:48:18 PM
// This file contains the implementations for Knight
// See Knight.h for documentation

#include "Knight.h"
#include "Board.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	Knight::Knight(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Knight::~Knight() = default;

	PieceType Knight::getType() const
	{
		return PieceType::Knight;
	}

	std::vector<Position> Knight::getLegalMoves(Board const& board) const
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

		checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file - 2));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file - 2));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 2));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 2));

		return legalPositions;
	}
}
}
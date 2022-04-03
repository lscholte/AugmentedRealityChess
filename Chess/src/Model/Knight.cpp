// Author:	Liam Scholte
// Created:	3/26/2022 11:48:18 PM
// This file contains the implementations for Knight
// See Knight.h for documentation

#include <Chess/Model/Knight.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	Knight::Knight(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	Knight::Knight(Knight const& otherKnight)
		: Piece(otherKnight)
	{}

	Knight::~Knight() = default;

	std::shared_ptr<Piece> Knight::clone() const
	{
		return std::make_shared<Knight>(*this);
	}

	PieceType Knight::getType() const
	{
		return PieceType::Knight;
	}

	std::vector<Position> Knight::getAttackingPositions(Board const& board) const
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

		checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file - 2));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file - 2));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 2));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 2));

		return attackingPositions;
	}
}
}
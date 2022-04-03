// Author:	Liam Scholte
// Created:	3/26/2022 11:48:47 PM
// This file contains the implementations for King
// See King.h for documentation

#include <Chess/Model/King.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>

namespace Chess
{
namespace Model
{
	King::King(bool isWhite, Position position)
		: Piece(isWhite, position)
	{}

	King::King(King const& otherKing)
		: Piece(otherKing)
	{}

	King::~King() = default;

	std::shared_ptr<Piece> King::clone() const
	{
		return std::make_shared<King>(*this);
	}

	PieceType King::getType() const
	{
		return PieceType::King;
	}

	std::vector<Position> King::getAttackingPositions(Board const& board) const
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

		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 0));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank + 0, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 1));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 0));
		checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 0, currentPosition.file - 1));
		checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file - 1));

		//TODO: Can also castle under certain circumstances (ignore this for now)

		return attackingPositions;
	}
}
}
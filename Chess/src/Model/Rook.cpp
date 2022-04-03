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

	Rook::Rook(Rook const& otherRook)
		: Piece(otherRook)
	{}

	Rook::~Rook() = default;

	std::shared_ptr<Piece> Rook::clone() const
	{
		return std::make_shared<Rook>(*this);
	}

	PieceType Rook::getType() const
	{
		return PieceType::Rook;
	}

	std::vector<Position> Rook::getAttackingPositions(Board const& board) const
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

		return attackingPositions;
	}
}
}
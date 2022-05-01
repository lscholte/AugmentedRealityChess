// Author:	Liam Scholte
// Created:	3/26/2022 11:47:52 PM
// This file contains the implementations for Pawn
// See Pawn.h for documentation

#include <Chess/Model/Pawn.h>
#include <Chess/Model/Board.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Size.h>

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

		Impl(Impl const& otherImpl)
			: isFirstMove(otherImpl.isFirstMove)
		{}
	};

	Pawn::Pawn(bool isWhite, Position position)
		: Piece(isWhite, position)
		, m_pImpl(std::make_unique<Impl>())
	{}

	Pawn::Pawn(Pawn const& otherPawn)
		: Piece(otherPawn)
		, m_pImpl(std::make_unique<Impl>(*otherPawn.m_pImpl))
	{}

	Pawn::~Pawn() = default;

	std::shared_ptr<Piece> Pawn::clone() const
	{
		return std::make_shared<Pawn>(*this);
	}

	PieceType Pawn::getType() const
	{
		return PieceType::Pawn;
	}

	std::vector<Position> Pawn::getAttackingPositions(Board const& board) const
	{
		std::vector<Position> attackingPositions;

		Position currentPosition = getPosition();

		auto checkAndPushPosition = [isWhite = isWhite(), currentPosition, &attackingPositions, &board](Position positionToCheck)
		{
			std::shared_ptr<Piece> pPiece = board.getPiece(positionToCheck);
			if (board.isPositionPossible(isWhite, positionToCheck))
			{
				if ((pPiece && positionToCheck.file != currentPosition.file) ||
					(!pPiece && positionToCheck.file == currentPosition.file))
				{
					attackingPositions.push_back(positionToCheck);
				}
			}
		};

		//Otherwise pawn can move 1 square forward
		//or 1 square diagonally if an opposing piece is there for capture
		if (isWhite())
		{
			checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file));
			checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file + 1));
			checkAndPushPosition(Position(currentPosition.rank + 1, currentPosition.file - 1));

			//Pawn can move 2 squares forward on first move
			//If there is nothing in the way
			if (m_pImpl->isFirstMove)
			{
				if (!board.getPiece(Position(currentPosition.rank + 1, currentPosition.file)))
				{
					checkAndPushPosition(Position(currentPosition.rank + 2, currentPosition.file));
				}
			}
		}
		else
		{
			checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file));
			checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file + 1));
			checkAndPushPosition(Position(currentPosition.rank - 1, currentPosition.file - 1));

			//Pawn can move 2 squares forward on first move
			//If there is nothing in the way
			if (m_pImpl->isFirstMove)
			{
				if (!board.getPiece(Position(currentPosition.rank - 1, currentPosition.file)))
				{
					checkAndPushPosition(Position(currentPosition.rank - 2, currentPosition.file));
				}
			}
		}

		//TODO: There is also en passant move (ignore this for now)

		return attackingPositions;
	}

	bool Pawn::move(Board& board, Position newPosition)
	{
		bool isMoveSuccessful = Piece::move(board, newPosition);

		if (isMoveSuccessful)
		{
			m_pImpl->isFirstMove = false;

			//If pawn has moved to the other side of the board, then promote it
			if ((isWhite() && newPosition.rank == board.getSize().ranks) ||
				(!isWhite() && newPosition.rank == 1))
			{
				//TODO: Need to allow for promotion to piece's other than Queen
				board.promotePiece(shared_from_this(), PieceType::Queen);
			}
		}
		return isMoveSuccessful;
	}
}
}
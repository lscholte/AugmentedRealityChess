// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the implementations for Board
// See Board.h for documentation

#include <Chess/Model/Board.h>
#include <Chess/Model/Pawn.h>
#include <Chess/Model/Rook.h>
#include <Chess/Model/Knight.h>
#include <Chess/Model/Bishop.h>
#include <Chess/Model/Queen.h>
#include <Chess/Model/King.h>
#include <Chess/Model/Position.h>

#include <memory>

namespace Chess
{
namespace Model
{
	struct Board::Impl
	{
		std::vector<std::shared_ptr<Piece>> pieces;

		Impl()
		{			
			//White
			pieces.push_back(std::make_shared<Rook>(true,	Position(1, 1)));
			pieces.push_back(std::make_shared<Knight>(true, Position(1, 2)));
			pieces.push_back(std::make_shared<Bishop>(true, Position(1, 3)));
			pieces.push_back(std::make_shared<Queen>(true,	Position(1, 4)));
			pieces.push_back(std::make_shared<King>(true,	Position(1, 5)));
			pieces.push_back(std::make_shared<Bishop>(true, Position(1, 6)));
			pieces.push_back(std::make_shared<Knight>(true, Position(1, 7)));
			pieces.push_back(std::make_shared<Rook>(true,	Position(1, 8)));

			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 1)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 2)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 3)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 4)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 5)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 6)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 7)));
			pieces.push_back(std::make_shared<Pawn>(true, Position(2, 8)));

			//Black
			pieces.push_back(std::make_shared<Rook>(false,		Position(8, 1)));
			pieces.push_back(std::make_shared<Knight>(false,	Position(8, 2)));
			pieces.push_back(std::make_shared<Bishop>(false,	Position(8, 3)));
			pieces.push_back(std::make_shared<Queen>(false,		Position(8, 4)));
			pieces.push_back(std::make_shared<King>(false,		Position(8, 5)));
			pieces.push_back(std::make_shared<Bishop>(false,	Position(8, 6)));
			pieces.push_back(std::make_shared<Knight>(false,	Position(8, 7)));
			pieces.push_back(std::make_shared<Rook>(false,		Position(8, 8)));

			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 1)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 2)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 3)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 4)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 5)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 6)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 7)));
			pieces.push_back(std::make_shared<Pawn>(false, Position(7, 8)));
		}
	};

	Board::Board()
		: m_pImpl(std::make_shared<Impl>())
	{}

	Board::~Board() = default;

	std::vector<std::shared_ptr<Piece>> const& Board::getPieces() const
	{
		return m_pImpl->pieces;
	}

	std::shared_ptr<Piece> Board::getPiece(Position position) const
	{
		auto iter = std::find_if(
			m_pImpl->pieces.cbegin(),
			m_pImpl->pieces.cend(),
			[position](std::shared_ptr<Piece> pPiece)
			{
				return pPiece->getPosition() == position;
			});

		return iter == m_pImpl->pieces.cend()
			? nullptr
			: *iter;
	}

	bool Board::isPositionOnBoard(Position position) const
	{
		if (position.rank < 1 ||
			position.file < 1 ||
			position.rank > 8 ||
			position.file > 8)
		{
			return false;
		}
		return true;
	}

	bool Board::isPositionLegal(bool isWhite, Position position) const
	{
		if (!isPositionOnBoard(position))
		{
			//Position is outside chessboard boundary
			return false;
		}

		auto iter = std::find_if(
			m_pImpl->pieces.cbegin(),
			m_pImpl->pieces.cend(),
			[isWhite, position](std::shared_ptr<Piece> pPiece)
			{
				return pPiece->isWhite() == isWhite && pPiece->getPosition() == position;
			});
		if (iter != m_pImpl->pieces.cend())
		{
			//A piece of the same color exists in the position
			return false;
		}

		//TODO: Check if King is in check

		return true;
	}
}
}

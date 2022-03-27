// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the implementations for Board
// See Board.h for documentation

#include "Board.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include "Position.h"

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
}
}

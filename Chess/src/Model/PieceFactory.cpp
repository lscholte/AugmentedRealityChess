// Author:	Liam Scholte
// Created:	4/9/2022 12:37:31 PM
// This file contains the implementations for PieceFactory
// See PieceFactory.h for documentation

#include <Chess/Model/PieceFactory.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Piece.h>
#include <Chess/Model/Pawn.h>
#include <Chess/Model/Rook.h>
#include <Chess/Model/Knight.h>
#include <Chess/Model/Bishop.h>
#include <Chess/Model/Queen.h>
#include <Chess/Model/King.h>

namespace Chess
{
namespace Model
{
	PieceFactory::PieceFactory(bool isWhite)
		: m_isWhite(isWhite)
	{}

	PieceFactory::~PieceFactory() = default;

	std::shared_ptr<Piece> PieceFactory::create(PieceType type, Position position) const
	{
		switch (type)
		{
		case PieceType::Pawn:
			return std::make_shared<Pawn>(m_isWhite, position);
		case PieceType::Rook:
			return std::make_shared<Rook>(m_isWhite, position);
		case PieceType::Knight:
			return std::make_shared<Knight>(m_isWhite, position);
		case PieceType::Bishop:
			return std::make_shared<Bishop>(m_isWhite, position);
		case PieceType::Queen:
			return std::make_shared<Queen>(m_isWhite, position);
		case PieceType::King:
			return std::make_shared<King>(m_isWhite, position);
		}
		return nullptr;
	}
}
}

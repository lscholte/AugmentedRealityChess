// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the implementations for Piece
// See Piece.h for documentation

#include "Piece.h"
#include "Position.h"

namespace Chess
{
namespace Model
{
	struct Piece::Impl
	{
		bool isWhite;
		Position position;

		Impl(bool isWhite, Position position)
			: isWhite(isWhite)
			, position(position)
		{}
	};

	Piece::Piece(bool isWhite, Position position)
		: m_pImpl(std::make_shared<Impl>(isWhite, position))
	{}

	Piece::~Piece() = default;

	bool Piece::isWhite() const
	{
		return m_pImpl->isWhite;
	}

	Position Piece::getPosition() const
	{
		return m_pImpl->position;
	}
}
}

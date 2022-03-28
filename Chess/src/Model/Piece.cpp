// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the implementations for Piece
// See Piece.h for documentation

#include <Chess/Model/Piece.h>
#include <Chess/Model/Position.h>

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

	bool Piece::move(Board const& board, Position newPosition)
	{
		//TODO: Calculations would have likely already been previously calculated.
		//I can probably eliminate this repeated calculation with some clever caching
		std::vector<Position> legalPositions = getLegalMoves(board);
		auto legalPositionIter = std::find_if(
			legalPositions.cbegin(),
			legalPositions.cend(),
			[newPosition](Position position)
			{
				return position == newPosition;
			});
		if (legalPositionIter == legalPositions.cend())
		{
			//Position is not legal for this piece
			return false;
		}

		m_pImpl->position = newPosition;
		return true;
	}
}
}

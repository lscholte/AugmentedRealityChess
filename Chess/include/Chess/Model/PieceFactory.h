// Author:	Liam Scholte
// Created:	4/9/2022 12:37:31 PM
// This file contains the class definition for PieceFactory

#pragma once

#include <memory>

namespace Chess
{
namespace Model
{
	class Piece;
	enum class PieceType;
	struct Position;

	class PieceFactory
	{
	public:
		PieceFactory(bool isWhite);
		virtual ~PieceFactory();

		std::shared_ptr<Piece> create(PieceType type, Position position) const;

	private:
		bool m_isWhite;
	};

}
}

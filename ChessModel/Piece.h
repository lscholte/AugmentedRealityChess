// Author:	Liam Scholte
// Created:	3/26/2022 9:50:38 PM
// This file contains the class definition for Piece

#pragma once

#include "Macros.h"

#include <memory>

namespace Chess
{
namespace Model
{
	struct Position;

	enum class PieceType
	{
		Pawn, Rook, Knight, Bishop, Queen, King
	};

	class EXPORT Piece
	{
	public:
		Piece(bool isWhite, Position position);
		virtual ~Piece();

		virtual PieceType getType() const = 0;

		virtual bool isWhite() const;
		virtual Position getPosition() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}

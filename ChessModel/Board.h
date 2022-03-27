// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the class definition for Board

#pragma once

#include "Macros.h"

#include <vector>
#include <memory>

namespace Chess
{
namespace Model
{
	class Piece;

	class EXPORT Board
	{
	public:
		Board();
		virtual ~Board();

		std::vector<std::shared_ptr<Piece>> const& getPieces() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
}


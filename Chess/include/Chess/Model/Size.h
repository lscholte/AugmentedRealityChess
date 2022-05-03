// Author:	Liam Scholte
// Created:	3/28/2022 12:24:47 AM
// This file contains the class definition for Size

#pragma once

#include <Chess/Macros.h>

namespace Chess
{
namespace Model
{
	/// <summary>
	/// Represents the dimensions of a chessboard.
	/// </summary>
	struct EXPORT Size
	{
	public:
		virtual ~Size();

		unsigned char ranks;
		unsigned char files;

		Size(unsigned char ranks, unsigned char files);

	};
}
}

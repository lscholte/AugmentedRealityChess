// Author:	Liam Scholte
// Created:	3/28/2022 12:24:47 AM
// This file contains the implementations for Size
// See Size.h for documentation

#include <Chess/Model/Size.h>

namespace Chess
{
namespace Model
{
	Size::Size(unsigned char ranks, unsigned char files)
		: ranks(ranks)
		, files(files)
	{}

	Size::~Size() = default;
}
}

// Author:	Liam Scholte
// Created:	4/12/2022 6:36:50 PM
// This file contains the class definition for CompositeDrawableObject

#pragma once

#include <Chess/ArView/DrawableObject.h>

#include <vector>
#include <memory>

namespace Chess
{
namespace ArView
{
	class CompositeDrawableObject
		: public DrawableObject
	{
	public:
		CompositeDrawableObject(std::vector<std::shared_ptr<DrawableObject>> const& drawableObjects);
		virtual ~CompositeDrawableObject();

		virtual void draw() const override;

	private:
		std::vector<std::shared_ptr<DrawableObject>> m_drawableObjects;

	};
}
}

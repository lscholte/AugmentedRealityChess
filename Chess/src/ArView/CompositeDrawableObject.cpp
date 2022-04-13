// Author:	Liam Scholte
// Created:	4/12/2022 6:37:35 PM
// This file contains the implementations for CompositeDrawableObject
// See CompositeDrawableObject.h for documentation

#include <Chess/ArView/CompositeDrawableObject.h>

namespace Chess
{
namespace ArView
{
	CompositeDrawableObject::CompositeDrawableObject(std::vector<std::shared_ptr<DrawableObject>> const& drawableObjects)
		: m_drawableObjects(drawableObjects)
	{}

	CompositeDrawableObject::~CompositeDrawableObject() = default;

	void CompositeDrawableObject::draw() const
	{
		for (auto const& pDrawableObject : m_drawableObjects)
		{
			pDrawableObject->draw();
		}
	}

}
}


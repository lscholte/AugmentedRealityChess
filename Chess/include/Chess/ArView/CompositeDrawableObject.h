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
	/// <summary>
	/// A DrawableObject that is composed of other DrawableObjects.
	/// </summary>
	class CompositeDrawableObject
		: public DrawableObject
	{
	public:

		/// <summary>
		/// Constructs a CompositeDrawableObject from a vector of DrawableObjects.
		/// </summary>
		/// <param name="drawableObjects">The objects that should be drawn when this object is drawn</param>
		CompositeDrawableObject(std::vector<std::shared_ptr<DrawableObject>> const& drawableObjects);

		virtual ~CompositeDrawableObject();

		virtual void draw() const override;

	private:
		std::vector<std::shared_ptr<DrawableObject>> m_drawableObjects;

	};
}
}

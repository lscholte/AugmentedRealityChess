// Author:	Liam Scholte
// Created:	3/13/2022 9:14:35 PM
// This file contains the class definition for DrawableObject

#pragma once

/// <summary>
/// An arbitrary object that can be drawn in a scene.
/// </summary>
class DrawableObject
{
public:
	virtual ~DrawableObject() = default;

	/// <summary>
	/// Draws this object onto the scene.
	/// </summary>
	virtual void draw() const = 0;

};

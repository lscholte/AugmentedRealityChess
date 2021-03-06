// Author:	Liam Scholte
// Created:	3/13/2022 10:50:41 PM
// This file contains the class definition for Quad

#pragma once

#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/Vertex.h>

#include <vector>

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// A quad that can be rendered over the entire image space.
	/// </summary>
	class Quad
		: public DrawableObject
	{
	public:
		Quad();
		Quad(std::vector<Vertex> const& vertices);

		virtual ~Quad();

		virtual void draw() const override;

	private:
		std::vector<Vertex> m_vertices;
		unsigned int m_vertexArrayObject, m_vertexBufferObject;
	};
}
}

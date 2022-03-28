// Author:	Liam Scholte
// Created:	3/13/2022 9:19:17 PM
// This file contains the class definition for LineSegments

#pragma once

#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/Vertex.h>

#include <vector>

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// A collection of line segments that can be drawn into a scene
	/// </summary>
	class LineSegments
		: public DrawableObject
	{
	public:

		/// <summary>
		/// Constructs LineSegments from a set of vertices.
		/// </summary>
		/// <param name="vertices"></param>
		LineSegments(std::vector<Vertex> const& vertices);

		virtual ~LineSegments();

		virtual void draw() const override;

	private:
		std::vector<Vertex> m_vertices;
		unsigned int m_vertexArrayObject, m_vertexBufferObject;
	};
}
}

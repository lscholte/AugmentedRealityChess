// Author:	Liam Scholte
// Created:	3/13/2022 9:19:17 PM
// This file contains the implementations for LineSegments
// See LineSegments.h for documentation

#include "LineSegments.h"
#include "Constants.h"

#include <gl/glew.h>

LineSegments::LineSegments(std::vector<Vertex> const& vertices)
	: m_vertices(vertices)
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(GLSL_POSITION_LOCATION);
	glVertexAttribPointer(GLSL_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(GLSL_COLOR_LOCATION);
	glVertexAttribPointer(GLSL_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LineSegments::~LineSegments() = default;

void LineSegments::draw() const
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_LINES, 0, m_vertices.size());
	glBindVertexArray(0);
}
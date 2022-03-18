// Author:	Liam Scholte
// Created:	3/13/2022 10:50:41 PM
// This file contains the implementations for Quad
// See Quad.h for documentation

#include "Quad.h"
#include "Vertex.h"
#include "VertexBuilder.h"
#include "Constants.h"

#include <glm/ext.hpp>
#include <gl/glew.h>

Quad::Quad()
{
	m_vertices =
	{
		VertexBuilder().addPosition(glm::vec3(-1.0f, -1.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 0.0f)).build(),
		VertexBuilder().addPosition(glm::vec3( 1.0f, -1.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 0.0f)).build(),
		VertexBuilder().addPosition(glm::vec3(-1.0f,  1.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 1.0f)).build(),
		VertexBuilder().addPosition(glm::vec3( 1.0f,  1.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 1.0f)).build(),
	};

	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(GLSL_POSITION_LOCATION);
	glVertexAttribPointer(GLSL_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(GLSL_TEXTURE_COORD_LOCATION);
	glVertexAttribPointer(GLSL_TEXTURE_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
}

Quad::~Quad() = default;

void Quad::draw() const
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices.size());
	glBindVertexArray(0);
}
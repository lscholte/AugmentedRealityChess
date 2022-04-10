// Author:	Liam Scholte
// Created:	3/12/2022 4:26:14 PM
// This file contains the implementations for DrawableObject
// See DrawableObject.h for documentation

#include <Chess/ArView/TriangleMesh.h>
#include <Chess/ArView/Constants.h>
#include <Chess/ArView/Vertex.h>

#include <gl/glew.h>

namespace Chess
{
namespace ArView
{
	TriangleMesh::TriangleMesh(
		std::vector<Vertex> const& vertices,
		std::vector<unsigned int> const& indices)
		: m_vertices(vertices)
		, m_indices(indices)
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glGenBuffers(1, &m_vertexBufferObject);
		glGenBuffers(1, &m_elementBufferObject);

		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(GLSL_POSITION_LOCATION);
		glVertexAttribPointer(GLSL_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(GLSL_COLOR_LOCATION);
		glVertexAttribPointer(GLSL_COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		glEnableVertexAttribArray(GLSL_NORMAL_LOCATION);
		glVertexAttribPointer(GLSL_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	TriangleMesh::~TriangleMesh() = default;

	void TriangleMesh::draw() const
	{
		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}
}
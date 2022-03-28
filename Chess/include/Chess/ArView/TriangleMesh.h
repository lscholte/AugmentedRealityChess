// Author:	Liam Scholte
// Created:	3/12/2022 4:26:14 PM
// This file contains the class definition for TriangleMesh

#pragma once

#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/Vertex.h>

#include <vector>

/// <summary>
/// Represents a triangle mesh, typically one that might be loaded from a file.
/// </summary>
class TriangleMesh
	: public DrawableObject
{
public:

	TriangleMesh(
		std::vector<Vertex> const& vertices,
		std::vector<unsigned int> const& indices);

	virtual ~TriangleMesh();

	virtual void draw() const override;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_vertexArrayObject, m_vertexBufferObject, m_elementBufferObject;
};

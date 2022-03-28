// Author:	Liam Scholte
// Created:	3/13/2022 7:12:16 PM
// This file contains the implementations for VertexBuilder
// See VertexBuilder.h for documentation

#include <Chess/ArView/VertexBuilder.h>

#include <glm/ext.hpp>


namespace Chess
{
namespace ArView
{
	VertexBuilder::VertexBuilder()
		: m_vertex()
	{}

	VertexBuilder::~VertexBuilder() = default;

	VertexBuilder& VertexBuilder::addPosition(glm::vec3 const& position)
	{
		m_vertex.position = position;
		return *this;
	}

	VertexBuilder& VertexBuilder::addColor(glm::vec3 const& color)
	{
		m_vertex.color = color;
		return *this;
	}

	VertexBuilder& VertexBuilder::addNormal(glm::vec3 const& normal)
	{
		m_vertex.normal = normal;
		return *this;
	}

	VertexBuilder& VertexBuilder::addTextureCoord(glm::vec2 const& textureCoord)
	{
		m_vertex.textureCoord = textureCoord;
		return *this;
	}

	Vertex VertexBuilder::build()
	{
		return m_vertex;
	}
}
}
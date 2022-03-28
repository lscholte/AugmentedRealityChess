// Author:	Liam Scholte
// Created:	3/13/2022 7:12:16 PM
// This file contains the class definition for VertexBuilder

#pragma once

#include <Chess/ArView/Vertex.h>

#include <glm/ext.hpp>

namespace Chess
{
namespace ArView
{
	/// <summary>
	/// A builder class for Vertex objects that allows constructing
	/// vertices with any combination of attributes set.
	/// </summary>
	class VertexBuilder
	{
	public:

		/// <summary>
		/// Constructs a VertexBuilder for an initially empty Vertex.
		/// </summary>
		VertexBuilder();

		virtual ~VertexBuilder();

		/// <summary>
		/// Adds a position to the vertex.
		/// </summary>
		/// <param name="position">The position to add</param>
		/// <returns>This builder object</returns>
		VertexBuilder& addPosition(glm::vec3 const& position);

		/// <summary>
		/// Adds a color to the vertex.
		/// </summary>
		/// <param name="color">The color to add</param>
		/// <returns>This builder object</returns>
		VertexBuilder& addColor(glm::vec3 const& color);

		/// <summary>
		/// Adds a normal to the vertex.
		/// </summary>
		/// <param name="normal">The normal to add</param>
		/// <returns>This builder object</returns>
		VertexBuilder& addNormal(glm::vec3 const& normal);

		/// <summary>
		/// Adds a texture coordinate to the vertex.
		/// </summary>
		/// <param name="textureCoord">The texture coordinate to add</param>
		/// <returns>This builder object</returns>
		VertexBuilder& addTextureCoord(glm::vec2 const& textureCoord);

		/// <summary>
		/// Returns the built Vertex object
		/// </summary>
		/// <returns>A Vertex object with the chosen attributes set</returns>
		Vertex build();

	private:
		Vertex m_vertex;
	};
}
}

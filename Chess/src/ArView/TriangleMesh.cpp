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
		std::vector<unsigned int> const& indices,
		std::vector<Texture> const& textures)
		: m_vertices(vertices)
		, m_indices(indices)
		, m_textures(textures)
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

		glEnableVertexAttribArray(GLSL_TEXTURE_COORD_LOCATION);
		glVertexAttribPointer(GLSL_TEXTURE_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	TriangleMesh::~TriangleMesh() = default;

	void TriangleMesh::draw() const
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < m_textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = m_textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to string
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string

			// now set the sampler to the correct texture unit
			unsigned int shaderId = 1;
			glUniform1i(glGetUniformLocation(shaderId, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}

		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}
}
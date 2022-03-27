// Author:	Liam Scholte
// Created:	3/10/2022 10:55:35 PM
// This file contains the implementations for ObjectDrawer
// See ObjectDrawer.h for documentation

#include "ObjectDrawer.h"
#include "ObjectLoader.h"
#include "Vertex.h"
#include "VertexBuilder.h"
#include "DrawableObject.h"
#include "LineSegments.h"
#include "Quad.h"
#include "Constants.h"

#include <Board.h>
#include <Piece.h>
#include <Position.h>

#include <Windows.h>

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <format>

#include <stb_image.h>

struct ObjectDrawer::Impl
{
	GLFWwindow* pWindow;
	GLuint imageShaderProgram;
	GLuint objectShaderProgram;

	size_t width, height;

	glm::mat4 projection;

	std::vector<std::shared_ptr<DrawableObject>> whitePawn;
	std::vector<std::shared_ptr<DrawableObject>> whiteRook;
	std::vector<std::shared_ptr<DrawableObject>> whiteKnight;
	std::vector<std::shared_ptr<DrawableObject>> whiteBishop;
	std::vector<std::shared_ptr<DrawableObject>> whiteQueen;
	std::vector<std::shared_ptr<DrawableObject>> whiteKing;

	std::vector<std::shared_ptr<DrawableObject>> blackPawn;
	std::vector<std::shared_ptr<DrawableObject>> blackRook;
	std::vector<std::shared_ptr<DrawableObject>> blackKnight;
	std::vector<std::shared_ptr<DrawableObject>> blackBishop;
	std::vector<std::shared_ptr<DrawableObject>> blackQueen;
	std::vector<std::shared_ptr<DrawableObject>> blackKing;

	std::shared_ptr<DrawableObject> pAxes;
	std::shared_ptr<DrawableObject> pQuad;

	std::shared_ptr<DrawableObject> pTable;
	GLuint tableTexture;

	Chess::Model::Board board;

	Impl(size_t width, size_t height)
		: width(width)
		, height(height)
	{
		glfwInit();
		glfwWindowHint(GLFW_SAMPLES, 4); //Enable anti-aliasing
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //Hide the window that appears

		pWindow = glfwCreateWindow(width, height, "Window", NULL, NULL);
		glfwMakeContextCurrent(pWindow);

		glewInit();

		imageShaderProgram = createShaderProgram("./shaders/ImageShader.vert", "./shaders/ImageShader.frag");
		objectShaderProgram = createShaderProgram("./shaders/ObjectShader.vert", "./shaders/ObjectShader.frag");

		projection = generateProjectionMatrix();

		//Black Chess Pieces
		glm::vec3 whiteColor(1.0f, 0.95f, 0.84f);
		{
			bool result = ObjectLoader().load("./assets/chess/pawn.stl", whitePawn, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/rook.stl", whiteRook, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/knight.stl", whiteKnight, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/bishop.stl", whiteBishop, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/queen.stl", whiteQueen, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/king.stl", whiteKing, whiteColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		
		//Black Chess Pieces
		glm::vec3 blackColor(0.1f, 0.1f, 0.1f);
		{
			bool result = ObjectLoader().load("./assets/chess/pawn.stl", blackPawn, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/rook.stl", blackRook, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/knight.stl", blackKnight, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/bishop.stl", blackBishop, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/queen.stl", blackQueen, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}
		{
			bool result = ObjectLoader().load("./assets/chess/king.stl", blackKing, blackColor);
			if (!result)
			{
				throw std::runtime_error("Failed to load mesh");
			}
		}

		//Create Axis vertices with particular positions and color
		std::vector<Vertex> axesVertices
		{
			VertexBuilder().addPosition(glm::vec3(0.0f, 0.0f, 0.0f)).addColor(glm::vec3(1.0f, 0.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(1.0f, 0.0f, 0.0f)).addColor(glm::vec3(1.0f, 0.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(0.0f, 0.0f, 0.0f)).addColor(glm::vec3(0.0f, 1.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(0.0f, 1.0f, 0.0f)).addColor(glm::vec3(0.0f, 1.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(0.0f, 0.0f, 0.0f)).addColor(glm::vec3(0.0f, 0.0f, 1.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(0.0f, 0.0f, 1.0f)).addColor(glm::vec3(0.0f, 0.0f, 1.0f)).build(),
		};

		pAxes = std::make_shared<LineSegments>(axesVertices);
		pQuad = std::make_shared<Quad>();


		std::vector<Vertex> quadVertices =
		{
			VertexBuilder().addPosition(glm::vec3(-1.0f, 1.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(10.0f, 1.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 0.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(-1.0f, -7.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 1.0f)).build(),
			VertexBuilder().addPosition(glm::vec3(10.0f, -7.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 1.0f)).build()
		};
		pTable = std::make_shared<Quad>(quadVertices);

		int imageWidth, imageHeight, nrComponents;
		unsigned char* imageData = stbi_load("./assets/tabletop.jpeg", &imageWidth, &imageHeight, &nrComponents, 0);

		glGenTextures(1, &tableTexture);
		glBindTexture(GL_TEXTURE_2D, tableTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint createShaderProgram(std::string const& vertexShaderPath, std::string const& fragmentShaderPath)
	{
		GLuint vertexShader = loadShader(vertexShaderPath, GL_VERTEX_SHADER);
		GLuint fragmentShader = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

		GLuint shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	GLuint loadShader(std::string const& shaderPath, GLenum shaderType)
	{
		std::ifstream fileStream(shaderPath, std::ios::in);
		if (!fileStream.is_open())
		{
			throw std::runtime_error("Cannot open shader");
		}

		std::stringstream contentStream;
		while (!fileStream.eof())
		{
			std::string line;
			std::getline(fileStream, line);
			contentStream << line << "\n";
		}

		GLuint shader = glCreateShader(shaderType);

		std::string content = contentStream.str();
		GLchar const * shaderSource = content.c_str();
		glShaderSource(shader, 1, &shaderSource, 0);

		glCompileShader(shader);
		GLint isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shader); // Don't leak the shader.
			
			throw std::runtime_error(errorLog.data());
		}

		return shader;
	}

	glm::mat4 generateProjectionMatrix()
	{
		//From https://amytabb.com/tips/tutorials/2019/06/28/OpenCV-to-OpenGL-tutorial-essentials/
		glm::mat4 projection(0.0f);
		projection[0][0] = -2.0 / width;
		projection[1][1] = 2.0 / height;
		projection[2][2] = -2.0 / (Z_FAR - Z_NEAR);
		projection[3][3] = 1.0;
		projection[3][0] = 1.0;
		projection[3][1] = -1.0;
		projection[3][2] = -(Z_FAR + Z_NEAR) / (Z_FAR - Z_NEAR);
		return projection;
	}

	std::vector<std::shared_ptr<DrawableObject>> getDrawableChessPiece(bool isWhite, Chess::Model::PieceType type)
	{
		if (isWhite)
		{
			switch (type)
			{
			case Chess::Model::PieceType::Pawn:		return whitePawn;
			case Chess::Model::PieceType::Rook:		return whiteRook;
			case Chess::Model::PieceType::Knight:	return whiteKnight;
			case Chess::Model::PieceType::Bishop:	return whiteBishop;
			case Chess::Model::PieceType::Queen:	return whiteQueen;
			case Chess::Model::PieceType::King:		return whiteKing;
			}
		}
		else
		{
			switch (type)
			{
			case Chess::Model::PieceType::Pawn:		return blackPawn;
			case Chess::Model::PieceType::Rook:		return blackRook;
			case Chess::Model::PieceType::Knight:	return blackKnight;
			case Chess::Model::PieceType::Bishop:	return blackBishop;
			case Chess::Model::PieceType::Queen:	return blackQueen;
			case Chess::Model::PieceType::King:		return blackKing;
			}
		}
	}
};

ObjectDrawer::ObjectDrawer(size_t width, size_t height)
	: m_pImpl(std::make_shared<Impl>(width, height))
{
}

ObjectDrawer::~ObjectDrawer() = default;

void ObjectDrawer::draw(unsigned char * imageData, glm::mat4 const& view)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	//Render real image
	{
		glUseProgram(m_pImpl->imageShaderProgram);

		//Load image data into a texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pImpl->width, m_pImpl->height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		m_pImpl->pQuad->draw();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glEnable(GL_DEPTH_TEST);

	//Render virtual objects onto scene
	{
		glUseProgram(m_pImpl->objectShaderProgram);

		GLint viewUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "View");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);

		GLint projectionUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Projection");
		glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &m_pImpl->projection[0][0]);

		//Render teapot
		{
			GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
			glUniform1i(hasImageUniformLocation, false);

			for (auto const& pChessPiece : m_pImpl->board.getPieces())
			{
				Chess::Model::Position position = pChessPiece->getPosition();

				float constexpr chessPieceScaleFactor = 0.025f;

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(position.file, -position.rank, 0.0f));
				model = pChessPiece->isWhite()
					? glm::rotate(model, -0.5f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f))
					: glm::rotate(model, 0.5f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(chessPieceScaleFactor));

				GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
				glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

				std::vector<std::shared_ptr<DrawableObject>> chessPieceParts = 
					m_pImpl->getDrawableChessPiece(pChessPiece->isWhite(), pChessPiece->getType());

				for (auto const& chessPiecePart : chessPieceParts)
				{
					chessPiecePart->draw();
				}
			}
		}

		////Render axes
		//{
		//	//Leave axes at origin
		//	glm::mat4 model(1.0f);
		//	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		//	GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
		//	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

		//	GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
		//	glUniform1i(hasImageUniformLocation, false);

		//	m_pImpl->pAxes->draw();
		//}

		//{
		//	glBindTexture(GL_TEXTURE_2D, m_pImpl->tableTexture);

		//	glm::mat4 model(1.0f);
		//	model = glm::translate(model, glm::vec3(-0.08f, 0.08f, 0.0f));

		//	GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
		//	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

		//	GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
		//	glUniform1i(hasImageUniformLocation, true);

		//	m_pImpl->pTable->draw();

		//	glBindTexture(GL_TEXTURE_2D, 0);
		//}
	}

	//Read the image data back
	glReadPixels(0, 0, m_pImpl->width, m_pImpl->height, GL_BGR, GL_UNSIGNED_BYTE, imageData);
}

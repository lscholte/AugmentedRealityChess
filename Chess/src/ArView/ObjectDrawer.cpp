// Author:	Liam Scholte
// Created:	3/10/2022 10:55:35 PM
// This file contains the implementations for ObjectDrawer
// See ObjectDrawer.h for documentation

#include <Chess/ArView/ObjectDrawer.h>
#include <Chess/ArView/ObjectLoader.h>
#include <Chess/ArView/Vertex.h>
#include <Chess/ArView/VertexBuilder.h>
#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/LineSegments.h>
#include <Chess/ArView/Quad.h>
#include <Chess/ArView/Constants.h>

#include <Chess/Model/Board.h>
#include <Chess/Model/Piece.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Game.h>
#include <Chess/Model/Size.h>

#include <Chess/Controller/Controller.h>

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

namespace Chess
{
namespace ArView
{
	struct ObjectDrawer::Impl
	{
		GLFWwindow* pWindow;
		GLuint imageShaderProgram;
		GLuint objectShaderProgram;
		GLuint idShaderProgram;

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

		std::shared_ptr<DrawableObject> pQuad;

		std::shared_ptr<DrawableObject> pLegalMoveSquare;
		std::shared_ptr<DrawableObject> pSelectedPieceSquare;

		std::shared_ptr<DrawableObject> pDrawableChessboard;
		GLuint chessboardTexture;

		Controller::Controller controller;

		//TODO: Replace with maps or a bidirectional map
		std::vector<std::pair<glm::u8vec3, Model::Position>> colorPositionMap;
		std::vector<std::pair<Model::Position, glm::u8vec3>> positionColorMap;

		GLuint idFramebuffer;

		Impl(size_t width, size_t height, Controller::Controller const& controller)
			: width(width)
			, height(height)
			, controller(controller)
		{
			glfwInit();
			glfwWindowHint(GLFW_SAMPLES, 4); //Enable anti-aliasing
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //Hide the window that appears

			pWindow = glfwCreateWindow(width, height, "Window", NULL, NULL);
			glfwMakeContextCurrent(pWindow);

			glewInit();

			imageShaderProgram = createShaderProgram("./shaders/ImageShader.vert", "./shaders/ImageShader.frag");
			objectShaderProgram = createShaderProgram("./shaders/ObjectShader.vert", "./shaders/ObjectShader.frag");
			idShaderProgram = createShaderProgram("./shaders/IdShader.vert", "./shaders/IdShader.frag");

			projection = generateProjectionMatrix();

			//White Chess Pieces
			glm::vec3 whiteColor(0.90f, 0.80f, 0.60f);
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
			glm::vec3 blackColor(0.3f, 0.2f, 0.17f);
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

			{
				glm::vec4 highlightColor(0.0f, 1.0f, 0.0f, 0.5f);
				std::vector<Vertex> chessboardSquareVertices =
				{
					VertexBuilder().addPosition(glm::vec3(-0.5f, -0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(-0.5f, 0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, -0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, 0.5f, 0.0f)).addColor(highlightColor).build()
				};
				pLegalMoveSquare = std::make_shared<Quad>(chessboardSquareVertices);
			}

			{
				glm::vec4 highlightColor(0.0f, 0.0f, 1.0f, 0.5f);
				std::vector<Vertex> chessboardSquareVertices =
				{
					VertexBuilder().addPosition(glm::vec3(-0.5f, -0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(-0.5f, 0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, -0.5f, 0.0f)).addColor(highlightColor).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, 0.5f, 0.0f)).addColor(highlightColor).build()
				};
				pSelectedPieceSquare = std::make_shared<Quad>(chessboardSquareVertices);
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

			pQuad = std::make_shared<Quad>();

			std::vector<Vertex> quadVertices =
			{
				VertexBuilder().addPosition(glm::vec3(0.0f, 0.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 0.0f)).build(),
				VertexBuilder().addPosition(glm::vec3(8.0f, 0.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 0.0f)).build(),
				VertexBuilder().addPosition(glm::vec3(0.0f, -8.0f, 0.0f)).addTextureCoord(glm::vec2(0.0f, 1.0f)).build(),
				VertexBuilder().addPosition(glm::vec3(8.0f, -8.0f, 0.0f)).addTextureCoord(glm::vec2(1.0f, 1.0f)).build()
			};
			pDrawableChessboard = std::make_shared<Quad>(quadVertices);

			int imageWidth, imageHeight, nrComponents;
			unsigned char* imageData = stbi_load("./assets/chess/chessboard.png", &imageWidth, &imageHeight, &nrComponents, 0);

			glGenTextures(1, &chessboardTexture);
			glBindTexture(GL_TEXTURE_2D, chessboardTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);

			for (unsigned char rank = 1; rank <= controller.getGame().getBoard().getSize().ranks; ++rank)
			{
				for (unsigned char file = 1; file <= controller.getGame().getBoard().getSize().files; ++file)
				{
					glm::u8vec3 color(32 * rank - 1, 0, 32 * file - 1);
					colorPositionMap.push_back(std::make_pair(color, Model::Position(rank, file)));
					positionColorMap.push_back(std::make_pair(Model::Position(rank, file), color));
				}
			}

			//Generate a framebuffer to render a texture to.
			//This is used to render chess squares a particular color to ID them
			glGenFramebuffers(1, &idFramebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, idFramebuffer);
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
			GLchar const* shaderSource = content.c_str();
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

		std::vector<std::shared_ptr<DrawableObject>> getDrawableChessPiece(bool isWhite, Model::PieceType type)
		{
			if (isWhite)
			{
				switch (type)
				{
				case Model::PieceType::Pawn:	return whitePawn;
				case Model::PieceType::Rook:	return whiteRook;
				case Model::PieceType::Knight:	return whiteKnight;
				case Model::PieceType::Bishop:	return whiteBishop;
				case Model::PieceType::Queen:	return whiteQueen;
				case Model::PieceType::King:	return whiteKing;
				}
			}
			else
			{
				switch (type)
				{
				case Model::PieceType::Pawn:	return blackPawn;
				case Model::PieceType::Rook:	return blackRook;
				case Model::PieceType::Knight:	return blackKnight;
				case Model::PieceType::Bishop:	return blackBishop;
				case Model::PieceType::Queen:	return blackQueen;
				case Model::PieceType::King:	return blackKing;
				}
			}
		}
	};

	ObjectDrawer::ObjectDrawer(size_t width, size_t height, Controller::Controller const& controller)
		: m_pImpl(std::make_shared<Impl>(width, height, controller))
	{
	}

	ObjectDrawer::~ObjectDrawer() = default;

	void ObjectDrawer::draw(unsigned char* imageData, glm::mat4 const& view, glm::vec3 const& cameraPosition)
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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		//Render virtual objects onto scene
		{
			glUseProgram(m_pImpl->objectShaderProgram);

			GLint viewUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "View");
			glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);

			GLint projectionUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Projection");
			glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &m_pImpl->projection[0][0]);

			GLint cameraPositionUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "CameraWorldPosition");
			glUniform3fv(cameraPositionUniformLocation, 1, glm::value_ptr(cameraPosition));
			
			//Render chessboard
			{
				glBindTexture(GL_TEXTURE_2D, m_pImpl->chessboardTexture);

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));

				GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
				glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

				GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
				glUniform1i(hasImageUniformLocation, true);

				m_pImpl->pDrawableChessboard->draw();

				glBindTexture(GL_TEXTURE_2D, 0);
			}

			//Render chess pieces
			{
				GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
				glUniform1i(hasImageUniformLocation, false);

				for (auto const& pChessPiece : m_pImpl->controller.getGame().getBoard().getPieces())
				{
					Model::Position position = pChessPiece->getPosition();

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

			//Render selected piece's legal moves
			{
				std::shared_ptr<Model::Piece> pSelectedPiece = m_pImpl->controller.getSelectedPiece();
				if (pSelectedPiece)
				{
					std::vector<Model::Position> legalMoves = pSelectedPiece->getLegalMoves(m_pImpl->controller.getGame().getBoard());
					for (auto const& position : legalMoves)
					{
						glm::mat4 model(1.0f);
						model = glm::translate(model, glm::vec3(position.file, -position.rank, 0.001f));

						GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
						glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

						m_pImpl->pLegalMoveSquare->draw();
					}
					{
						Model::Position position = pSelectedPiece->getPosition();
						glm::mat4 model(1.0f);
						model = glm::translate(model, glm::vec3(position.file, -position.rank, 0.001f));

						GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
						glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

						m_pImpl->pSelectedPieceSquare->draw();
					}
				}
			}
		}

		glDisable(GL_BLEND);

		//Render into ID image
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pImpl->idFramebuffer);
			glClear(GL_COLOR);

			glUseProgram(m_pImpl->idShaderProgram);

			GLint viewUniformLocation = glGetUniformLocation(m_pImpl->idShaderProgram, "View");
			glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);

			GLint projectionUniformLocation = glGetUniformLocation(m_pImpl->idShaderProgram, "Projection");
			glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &m_pImpl->projection[0][0]);

			for (auto const& positionColorPair : m_pImpl->positionColorMap)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(positionColorPair.first.file, -positionColorPair.first.rank, 0.0f));

				GLint modelUniformLocation = glGetUniformLocation(m_pImpl->idShaderProgram, "Model");
				glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

				glm::vec3 color(positionColorPair.second.r / 255.0f, positionColorPair.second.g / 255.0f, positionColorPair.second.b / 255.0f);

				std::vector<Vertex> chessboardSquareVertices =
				{
					VertexBuilder().addPosition(glm::vec3(-0.5f, -0.5f, 0.0f)).addColor(color).build(),
					VertexBuilder().addPosition(glm::vec3(-0.5f, 0.5f, 0.0f)).addColor(color).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, -0.5f, 0.0f)).addColor(color).build(),
					VertexBuilder().addPosition(glm::vec3(0.5f, 0.5f, 0.0f)).addColor(color).build()
				};
				auto square = std::make_shared<Quad>(chessboardSquareVertices);

				square->draw();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		//Read the image data back
		glReadPixels(0, 0, m_pImpl->width, m_pImpl->height, GL_BGR, GL_UNSIGNED_BYTE, imageData);
	}

	std::optional<Model::Position> ObjectDrawer::handleClick(float x, float y)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_pImpl->idFramebuffer);

		glm::u8vec3 color;
		glReadPixels((GLint)(x * m_pImpl->width), (GLint)(y * m_pImpl->height), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color[0]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		auto iter = std::find_if(
			m_pImpl->colorPositionMap.cbegin(),
			m_pImpl->colorPositionMap.cend(),
			[color](std::pair<glm::u8vec3, Model::Position> const& pair)
			{
				return color == pair.first;
			});

		if (iter == m_pImpl->colorPositionMap.cend())
		{
			return std::nullopt;
		}

		return iter->second;
	}
}
}

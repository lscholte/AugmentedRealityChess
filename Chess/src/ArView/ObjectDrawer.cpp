// Author:	Liam Scholte
// Created:	3/10/2022 10:55:35 PM
// This file contains the implementations for ObjectDrawer
// See ObjectDrawer.h for documentation

#include <Chess/ArView/ObjectDrawer.h>
#include <Chess/ArView/ObjectLoader.h>
#include <Chess/ArView/Vertex.h>
#include <Chess/ArView/VertexBuilder.h>
#include <Chess/ArView/DrawableObject.h>
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
#include <boost/functional/hash.hpp>

#include <unordered_map>

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

		std::unordered_map<
			std::pair<Model::PieceType, bool>,
			std::shared_ptr<DrawableObject>,
			boost::hash<std::pair<Model::PieceType, bool>>> chessPieceMap;

		std::shared_ptr<DrawableObject> pQuad;

		std::shared_ptr<DrawableObject> pLegalMoveSquare;
		std::shared_ptr<DrawableObject> pSelectedPieceSquare;

		std::shared_ptr<DrawableObject> pDrawableChessboard;
		GLuint chessboardTexture;

		std::shared_ptr<Controller::Controller> pController;

		//TODO: Replace with maps or a bidirectional map
		std::vector<std::pair<glm::u8vec3, Model::Position>> colorPositionMap;
		std::vector<std::pair<Model::Position, glm::u8vec3>> positionColorMap;

		GLuint idFramebuffer;

		ObjectLoader objectLoader;

		Impl(size_t width, size_t height, std::shared_ptr<Controller::Controller> pController)
			: width(width)
			, height(height)
			, pController(pController)
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
			std::vector<std::pair<Model::PieceType, std::string>> whiteChessPieceAssetPaths =
			{
				{ Model::PieceType::Pawn, "./assets/pieces/white/pawn.obj" },
				{ Model::PieceType::Rook, "./assets/pieces/white/rook.obj" },
				{ Model::PieceType::Knight, "./assets/pieces/white/knight.obj" },
				{ Model::PieceType::Bishop, "./assets/pieces/white/bishop.obj" },
				{ Model::PieceType::Queen, "./assets/pieces/white/queen.obj" },
				{ Model::PieceType::King, "./assets/pieces/white/king.obj" }
			};
			for (auto const& chessPieceAssetPath : whiteChessPieceAssetPaths)
			{
				loadChessPiece(chessPieceAssetPath.first, chessPieceAssetPath.second, true);
			}

			//Black Chess Pieces
			std::vector<std::pair<Model::PieceType, std::string>> blackChessPieceAssetPaths =
			{
				{ Model::PieceType::Pawn, "./assets/pieces/black/pawn.obj" },
				{ Model::PieceType::Rook, "./assets/pieces/black/rook.obj" },
				{ Model::PieceType::Knight, "./assets/pieces/black/knight.obj" },
				{ Model::PieceType::Bishop, "./assets/pieces/black/bishop.obj" },
				{ Model::PieceType::Queen, "./assets/pieces/black/queen.obj" },
				{ Model::PieceType::King, "./assets/pieces/black/king.obj" }
			};
			for (auto const& chessPieceAssetPath : blackChessPieceAssetPaths)
			{
				loadChessPiece(chessPieceAssetPath.first, chessPieceAssetPath.second, false);
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

			pQuad = std::make_shared<Quad>();

			pDrawableChessboard = ObjectLoader().load("./assets/chessboard/chessboard.obj");

			for (unsigned char rank = 1; rank <= pController->getGame().getBoard().getSize().ranks; ++rank)
			{
				for (unsigned char file = 1; file <= pController->getGame().getBoard().getSize().files; ++file)
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

		std::shared_ptr<DrawableObject> getDrawableChessPiece(bool isWhite, Model::PieceType type)
		{
			return chessPieceMap.find({ type, isWhite })->second;
		}

		void loadChessPiece(Model::PieceType type, std::string const& assetPath, bool isWhite)
		{
			std::shared_ptr<DrawableObject> pChessPiece = objectLoader.load(assetPath);
			if (!pChessPiece)
			{
				throw std::runtime_error("Failed to load mesh: " + assetPath);
			}
			chessPieceMap[{ type, isWhite }] = pChessPiece;
		}
	};

	ObjectDrawer::ObjectDrawer(size_t width, size_t height, std::shared_ptr<Controller::Controller> pController)
		: m_pImpl(std::make_unique<Impl>(width, height, pController))
	{
	}

	ObjectDrawer::~ObjectDrawer() = default;

	void ObjectDrawer::draw(unsigned char* imageData, unsigned char* depthData, glm::mat4 const& view, glm::vec3 const& cameraPosition)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		glm::vec3 translationVector(-0.5f, -0.5f, 0.0f);

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

		float chessboardHeight = 0.5f;

		//Render virtual objects onto scene
		{
			glUseProgram(m_pImpl->objectShaderProgram);

			GLuint texture;
			glActiveTexture(GL_TEXTURE15);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_pImpl->width, m_pImpl->height, 0, GL_RED, GL_UNSIGNED_BYTE, depthData);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glUniform1i(glGetUniformLocation(m_pImpl->objectShaderProgram, "AlphaMask"), 15);

			GLint viewUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "View");
			glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);

			GLint projectionUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Projection");
			glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &m_pImpl->projection[0][0]);

			GLint cameraPositionUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "CameraWorldPosition");
			glUniform3fv(cameraPositionUniformLocation, 1, glm::value_ptr(cameraPosition));
			
			//Render chessboard
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, translationVector);

				GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
				glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

				GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
				glUniform1i(hasImageUniformLocation, true);
				
				m_pImpl->pDrawableChessboard->draw();
			}

			//Render chess pieces
			{
				for (auto const& pChessPiece : m_pImpl->pController->getGame().getBoard().getPieces())
				{
					Model::Position position = pChessPiece->getPosition();

					GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
					glUniform1i(hasImageUniformLocation, true);

					float constexpr chessPieceScaleFactor = 0.025f;
					
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(position.file, position.rank, chessboardHeight));
					model = glm::translate(model, translationVector);
					if (pChessPiece->getType() == Model::PieceType::Knight)
					{
						model = pChessPiece->isWhite()
							? glm::rotate(model, 0.5f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f))
							: glm::rotate(model, -0.5f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f));
					}
					model = glm::scale(model, glm::vec3(chessPieceScaleFactor));

					GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
					glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

					m_pImpl->getDrawableChessPiece(pChessPiece->isWhite(), pChessPiece->getType())->draw();
				}
			}

			//Render selected piece's legal moves
			{
				GLint hasImageUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "HasImage");
				glUniform1i(hasImageUniformLocation, false);

				std::shared_ptr<Model::Piece const> pSelectedPiece = m_pImpl->pController->getSelectedPiece();
				if (pSelectedPiece)
				{
					std::vector<Model::Position> legalMoves = pSelectedPiece->getLegalMoves(m_pImpl->pController->getGame().getBoard());
					for (auto const& position : legalMoves)
					{
						glm::mat4 model(1.0f);
						model = glm::translate(model, glm::vec3(position.file, position.rank, chessboardHeight + 0.001f));
						model = glm::translate(model, translationVector);

						GLint modelUniformLocation = glGetUniformLocation(m_pImpl->objectShaderProgram, "Model");
						glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

						m_pImpl->pLegalMoveSquare->draw();
					}
					{
						Model::Position position = pSelectedPiece->getPosition();
						glm::mat4 model(1.0f);
						model = glm::translate(model, glm::vec3(position.file, position.rank, chessboardHeight + 0.001f));
						model = glm::translate(model, translationVector);

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
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(m_pImpl->idShaderProgram);

			GLint viewUniformLocation = glGetUniformLocation(m_pImpl->idShaderProgram, "View");
			glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);

			GLint projectionUniformLocation = glGetUniformLocation(m_pImpl->idShaderProgram, "Projection");
			glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &m_pImpl->projection[0][0]);

			for (auto const& positionColorPair : m_pImpl->positionColorMap)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(positionColorPair.first.file, positionColorPair.first.rank, chessboardHeight));
				model = glm::translate(model, translationVector);

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

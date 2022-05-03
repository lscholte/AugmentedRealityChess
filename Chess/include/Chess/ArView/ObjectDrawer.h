// Author:	Liam Scholte
// Created:	3/10/2022 10:55:35 PM
// This file contains the class definition for ObjectDrawer

#pragma once

#include <Chess/Model/FwdDecl.h>

#include <glm/ext.hpp>

#include <memory>
#include <optional>

namespace Chess
{
namespace Controller
{
	class Controller;
}

namespace ArView
{

	/// <summary>
	/// Draws virtual objects, in particular a teapot and xyz axes, onto an image.
	/// </summary>
	class ObjectDrawer
	{
	public:

		/// <summary>
		/// Constructs an ObjectDrawer targeting images with a specified width and height.
		/// </summary>
		/// <param name="width">The width of images that will be drawn on top of</param>
		/// <param name="height">The height of images that will be drawn on top of</param>
		/// <param name="pController">A pointer to the controller object for the chess game</param>
		ObjectDrawer(size_t width, size_t height, std::shared_ptr<Controller::Controller> pController);
		virtual ~ObjectDrawer();

		/// <summary>
		/// Draws the virtual objects into the scene.
		/// </summary>
		/// <param name="imageData">The image data in BGR format to draw onto, which will be modified</param>
		/// <param name="depthData">A binary image that acts as a mask for whether to discard fragments</param>
		/// <param name="view">Transformation matrix corresponding to the camera's intrinsic and extrinsic parameters</param>
		void draw(unsigned char* imageData, unsigned char* depthData, glm::mat4 const& view, glm::vec3 const& cameraPosition);

		/// <summary>
		/// Determines the chessboard square corresponding to a normalized position in the range of [0,1].
		/// </summary>
		/// <param name="x">The normalized x coordinate</param>
		/// <param name="y">The normalized y coordinate</param>
		/// <returns>
		/// A chessboard position corresponding to the image coordinate,
		/// or empty optional if no such position exists
		/// </returns>
		std::optional<Model::Position> handleClick(float x, float y);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}
}

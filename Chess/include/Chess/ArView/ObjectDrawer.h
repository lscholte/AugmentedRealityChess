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
		ObjectDrawer(size_t width, size_t height, Controller::Controller const& controller);
		virtual ~ObjectDrawer();

		/// <summary>
		/// Draws the virtual objects into the scene.
		/// </summary>
		/// <param name="imageData">The image data in BGR format to draw onto, which will be modified</param>
		/// <param name="view">Transformation matrix corresponding to the camera's intrinsic and extrinsic parameters</param>
		void draw(unsigned char* imageData, glm::mat4 const& view, glm::vec3 const& cameraPosition);

		std::optional<Model::Position> handleClick(float x, float y);

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;

	};
}
}

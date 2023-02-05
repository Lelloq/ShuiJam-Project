/*\file Rect.h
\brief 2D rectangle*/
#pragma once
#include <glm/glm.hpp>
#include <array>
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"

namespace SJ
{
	/*\class Rect
	\brief 2D rectangle class*/
	class Rect
	{
	private:
		int m_zIndex = 0;//!<Z index higher the number the further back it is so higher values are in front

		std::array<float, 20> m_verts = //!<Vertices
		//POSITIONS								  //UV_COORDS
		{0.f, 0.f, static_cast<float>(-m_zIndex), 0.0f, 0.0f,
		 1.f, 0.f, static_cast<float>(-m_zIndex), 1.0f, 0.0f,
		 1.f, 1.f, static_cast<float>(-m_zIndex), 1.0f, 1.0f,
		 0.f, 1.f, static_cast<float>(-m_zIndex), 0.0f, 1.0f,};

		std::array<unsigned, 6> m_indices = //!<Indices
		{
			3,2,1,
			3,1,0
		};

		glm::vec3 m_position{ 0.f };//!<Position of the rectangle
		glm::vec3 m_scale{ 1.0f };//!<Size of the rectangle
		float m_angle = 0.f;//!<Angle of rotation in degrees (rotate on z axis (0,0,1) - 2D game)
		std::shared_ptr<VAO> m_VAO;
		std::shared_ptr<VBO> m_VBO;
		std::shared_ptr<VBO> m_EBO;
		std::shared_ptr<Texture> m_texture;

	public:
		Rect();
		~Rect();

	};
}
/*\file Rect.h
\brief 2D rectangle*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
		std::array<float, 20> m_verts = //!<Default vertices
		//POSITION    //UV_COORDS
		{0.f, 0.f, 0, 0.0f, 0.0f,
		 1.f, 0.f, 0, 1.0f, 0.0f,
		 1.f, 1.f, 0, 1.0f, 1.0f,
		 0.f, 1.f, 0, 0.0f, 1.0f,};

		std::array<uint32_t, 6> m_indices = //!<Indices
		{
			0,1,3,
			3,1,2,
		};

		glm::vec2 m_position;//!<Position of the rectangle
		glm::vec2 m_size;//!<Size of the rectangle
		float m_angle = 0.f;//!<Angle of rotation in degrees (rotate on z axis (0,0,1) - 2D game)
		VAO* m_VAO;//!< Vertex array object
		VBO* m_VBO;//!< Vertex buffer object
		EBO* m_EBO;//!< Element buffer object
		Texture* m_texture;//!< Texture image
	public:
		/**
		 * Constructs a rectangle by taking in these parameters.
		 * \param pos - glm::vec2&, position of the rectangle origin is the top left
		 * \param size - glm::vec2&, size of the rectangle origin is in the top left
		 * \param zIndex - int, the z-index where the higher value is rendered infront of another in orthograhpic view
		 * \param image - Texture&, image is put in here and will be rendered on the rectangle
		 */
		Rect(const glm::vec2& pos, const glm::vec2& size, const int zIndex, Texture& image);
		~Rect();
		void Draw(Shader& shader);//!< Bind the buffer objects and texture slot and draw it to screen
		[[nodiscard]] inline VAO& getVAO() { return *m_VAO; }//!< Access the vao for rendering
		[[nodiscard]] inline EBO& getEBO() { return *m_EBO; }//!< Access the ebo for rendering
		[[nodiscard]] inline glm::vec2 getPosition() { return m_position; }
		[[nodiscard]] inline glm::vec2 getSize() { return m_size; }
	};
}
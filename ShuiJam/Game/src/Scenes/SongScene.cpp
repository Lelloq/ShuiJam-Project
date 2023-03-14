/*****************************************************************//**
 * \file   SongScene.cpp
 * \brief  Song select scene implementation
 * 
 * \date   February 2023
 *********************************************************************/
#include "Scenes/SongScene.h"
#include "Utils/Properties.h"
#include <GLFW/glfw3.h>

namespace SJ
{
	SongScene::SongScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		m_SFXscroll = std::make_unique<SFXSource>();
		m_SFXstart = std::make_unique<SFXSource>();

		m_songSelectIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "songselectmenu.png", GL_CLAMP_TO_EDGE);
		m_songSelect = std::make_unique<Rect>(glm::vec2(0.f,0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_songSelectIm);

		m_logoIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_logo = std::make_unique<Button>(glm::vec2(0.f, 0.f), glm::vec2(125.f, 125.f), 1, *m_logoIm);

		glm::mat4 model{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_shader->use();
		m_shader->setMat4("model", model);
		m_shader->setMat4("view", view);
		m_shader->setMat4("projection", projection);

		m_buttons.push_back(*m_logo);
	}
	void SongScene::Update(float dt)
	{

	}
	void SongScene::Render()
	{
		glClearColor(0.5568f, 0.8f, 0.7764f, 0.f);
		m_songSelect->Draw(*m_shader);
		m_logo->Draw(*m_shader);
	}
	void SongScene::getKey(int key, int scancode, int action, int mods)
	{

	}
	void SongScene::getMouseButton(int button, int action, int mods)
	{
		double posX, posY;
		glfwGetCursorPos(m_window, &posX, &posY);
		if(m_buttons.at(0).hasMouseOnTop(posX,posY))
		{
			std::cout << m_buttons.at(0).getZIndex() << "\n";
		}
	}
	void SongScene::getScroll(double xoffset, double yoffset)
	{

	}
	void SongScene::fileDrop(int count, const char** paths)
	{

	}
}

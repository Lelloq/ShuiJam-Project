/*****************************************************************//**
 * \file   GameScene.cpp
 * \date   March 2023
 *********************************************************************/
#include "Scenes/GameScene.h"
#include "Utils/Properties.h"
#include <future>

namespace SJ
{
	GameScene::GameScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		m_notes = std::async(std::launch::async, OsuParser::parse, g_CurrentOsuDir, g_CurrentDifficulty).get();
		m_music = std::make_unique<Music>(m_folder + g_CurrentOsuDir + L"/" + g_CurrentSong);

		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_shader->use();
		m_shader->setMat4("projection", projection);


	}

	void GameScene::Update(float dt)
	{
		
	}

	void GameScene::Render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void GameScene::getKey(int key, int scancode, int action, int mods)
	{
		for(int i = 0; i < 7; i++)
		{
			if(action == GLFW_PRESS && key == m_inputs.at(i))
			{

			}
			else if(action == GLFW_RELEASE && key == m_inputs.at(i))
			{

			}
		}
	}

	void GameScene::getMouseButton(int button, int action, int mods)
	{
	}

	void GameScene::getScroll(double xoffset, double yoffset)
	{
	}

	void GameScene::fileDrop(int count, const char** paths)
	{
	}
}

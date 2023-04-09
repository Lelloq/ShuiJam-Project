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

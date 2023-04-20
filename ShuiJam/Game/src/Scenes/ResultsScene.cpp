/*****************************************************************//**
 * \file   ResultsScene.cpp
 * \brief  the results screen
 * 
 * \date   April 2023
 *********************************************************************/
#include "Scenes/ResultsScene.h"
#include "Utils/Properties.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace SJ
{
	ResultsScene::ResultsScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{

	}

	void ResultsScene::Update(float dt)
	{
	}

	void ResultsScene::Render()
	{
	}

	void ResultsScene::getKey(int key, int scancode, int action, int mods)
	{
	}

	void ResultsScene::getMouseButton(int button, int action, int mods)
	{
	}

	void ResultsScene::getScroll(double xoffset, double yoffset)
	{
	}

	void ResultsScene::fileDrop(int count, const char** paths)
	{
	}
}

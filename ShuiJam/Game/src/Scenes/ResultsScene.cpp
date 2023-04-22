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
		//Reenable cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "click.mp3");

		//Create shaders
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->setMat4("projection", projection);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");
		m_textShader->setMat4("projection", projection);

		//Create textures
		m_songIm = std::make_unique<Texture>(m_folder + g_CurrentOsuDir + L"/" + g_CurrentSong, GL_CLAMP_TO_EDGE);
		m_gradesBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "resultsbg.png", GL_CLAMP_TO_EDGE);

		std::array<std::string, 8> gradePaths = { "GradeP.png", "GradeAAA.png" ,"GradeAA.png" ,"GradeA.png"
			,"GradeB.png" ,"GradeC.png" ,"GradeD.png" ,"GradeF.png" };
		for (int i = 0; i < 8; i++)
		{
			m_gradesIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + gradePaths.at(i), GL_CLAMP_TO_EDGE);
		}

		std::array<std::string, 5> judgeFiles =
		{ "perfect.png","great.png","good.png","bad.png","miss.png" };
		for (int i = 0; i < 5; i++)
		{
			m_judgementIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + judgeFiles.at(i), GL_CLAMP_TO_EDGE);
		}


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
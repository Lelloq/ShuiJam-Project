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

		//Create shader
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->use();
		m_shader->setMat4("projection", projection);

	#pragma region texture creation
		//Create textures
		for (int i = 0; i < 7; i++)
		{
			//Create note images
			m_riceIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/note" + static_cast<char>(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_headIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/head" + static_cast<char>(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_tailIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/tail" + static_cast<char>(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_bodyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/body" + static_cast<char>(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			//Key press images
			m_keyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/key" + static_cast<char>(i + 1) + ".png", GL_CLAMP_TO_EDGE);
		}
		//Stage images
		m_stageLeftIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stageleft.png", GL_CLAMP_TO_EDGE);
		m_stageRightIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stageright.png", GL_CLAMP_TO_EDGE);
		m_stageHitpositionIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stagehit.png", GL_CLAMP_TO_EDGE);
		m_stageBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stagebg.png", GL_CLAMP_TO_EDGE);
		//Health images
		m_healthIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/healthbar.png", GL_CLAMP_TO_EDGE);
		m_healthBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/healthbarbg.png", GL_CLAMP_TO_EDGE);
		//Numbers
		std::array<std::string, 10> numFiles =
		{ "zero.png","one.png","two.png","three.png","four.png","five.png","six.png","seven.png","eight.png","nine.png" };
		for (int i = 0; i < 10; i++)
		{
			m_numIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + numFiles.at(i), GL_CLAMP_TO_EDGE);
		}
		//Percent
		m_percentIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/percent.png", GL_CLAMP_TO_EDGE);
		m_dotIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/dot.png", GL_CLAMP_TO_EDGE);
		//Judgement
		std::array<std::string, 5> judgeFiles =
		{ "perfect.png","great.png","good.png","bad.png","miss.png" };
		for (int i = 0; i < 5; i++)
		{
			m_judgementIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + judgeFiles.at(i), GL_CLAMP_TO_EDGE);
		}
	#pragma endregion

	#pragma region object creation
		//Create objects
		//Stage
		//named middleBL since the textures are created where the origin is at the bottom left
		float middleBL = (VPORT_WIDTH / 2) - (m_stageBGIm->getWidth() / 2);
		m_stageBG = std::make_unique<Rect>(glm::vec2(middleBL, 0),glm::vec2(m_stageBGIm->getWidth(),VPORT_HEIGHT), 0, *m_stageBGIm);
		m_stageLeft = std::make_unique<Rect>(glm::vec2(middleBL - m_stageLeftIm->getWidth(), 0), glm::vec2(m_stageLeftIm->getWidth(), VPORT_HEIGHT), 1, *m_stageLeftIm);
		m_stageRight= std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth(), 0), glm::vec2(m_stageRightIm->getWidth(), VPORT_HEIGHT), 1, *m_stageRightIm);
		m_stageHitposition = std::make_unique<Rect>(glm::vec2(middleBL, m_hitPosition), glm::vec2(m_stageHitpositionIm->getWidth(), m_stageHitpositionIm->getHeight()), 1, *m_stageHitpositionIm);
		//Health
		m_health = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_healthIm->getWidth(), 0), glm::vec2(m_healthIm->getWidth(), m_healthIm->getHeight()), 2, *m_healthIm);
		m_healthBG = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_healthBGIm->getWidth(), 0), glm::vec2(m_healthBGIm->getWidth(), m_healthBGIm->getHeight()), 1, *m_healthBGIm);
		//Key
		int xPos = middleBL;
		for(int i = 0; i < 7; i++)
		{
			m_key.at(i) = std::make_unique<Rect>(glm::vec2(xPos, 0), glm::vec2(m_stageBGIm->getWidth() / 7, m_keyIm.at(i)->getHeight()), 3, *m_keyIm.at(i));
			xPos += m_stageBGIm->getWidth() / 7;
		}
		//Numbers
		for(int i = 0; i < 10; i++)
		{
			m_num.at(i) = std::make_unique<Rect>(glm::vec2(middleBL + (m_stageBGIm->getWidth() / 2), m_comboPosition), glm::vec2(m_numIm.at(i)->getWidth(), m_numIm.at(i)->getHeight()), 3, *m_numIm.at(i));
		}
		//Dot and Percent
		m_percent = std::make_unique<Rect>(glm::vec2(VPORT_WIDTH - m_percentIm->getWidth(), VPORT_HEIGHT - m_percentIm->getHeight()), glm::vec2(m_percentIm->getWidth(), m_percentIm->getHeight()), 3, *m_percentIm);
		//NOTE:Position for the dot will change as the accuracy changes
		m_dot = std::make_unique<Rect>(glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), glm::vec2(m_dotIm->getWidth(), m_dotIm->getHeight()), 3, *m_dotIm);
		//Judgement
		for(int i = 0; i < 5; i++)
		{
			m_judgement.at(i) = std::make_unique<Rect>
				(glm::vec2(middleBL + (m_stageBGIm->getWidth() / 2) - m_judgementIm.at(i)->getWidth(), m_judgePosition), 
				 glm::vec2(m_judgementIm.at(i)->getWidth(),m_judgementIm.at(i)->getHeight()), 3, *m_judgementIm.at(i));
		}

	#pragma endregion
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
		for (int i = 0; i < 7; i++)
		{
			if (action == GLFW_PRESS && key == m_inputs.at(i))
			{
			}
			else if (action == GLFW_RELEASE && key == m_inputs.at(i))
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
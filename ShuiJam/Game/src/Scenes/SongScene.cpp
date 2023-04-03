﻿/*****************************************************************//**
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
		m_fileProcessor = std::make_unique<FileProcessor>();

		m_SFXstart = std::make_unique<SFXSource>();
		m_scrollSound = m_sfx->addSFX(SJFOLDER + SOUNDS + "scroll.wav");

		m_songSelectIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "songselectmenu.png", GL_CLAMP_TO_EDGE);
		m_songSelect = std::make_unique<Rect>(glm::vec2(0.f,0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 2, *m_songSelectIm);

		m_logoIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_logo = std::make_unique<Button>(glm::vec2(0.f, 0.f), glm::vec2(125.f, 125.f), 3, *m_logoIm);

		m_songBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "selectbg.png", GL_CLAMP_TO_EDGE);
		m_songBG = std::make_unique<Rect>(glm::vec2(0.f, 0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_songBGIm);

		m_selectWheelIm = std::make_shared<Texture>(SJFOLDER + IMAGES + "selectbar.png", GL_CLAMP_TO_EDGE);

		int yPos = 0;
		for (int i = 0; i < 12; i++)
		{
			m_buttonPositions.push_back(630+yPos);
			m_buttons.push_back(std::make_unique<Button>(glm::vec2(829, 630+yPos), glm::vec2(451, 57), 0, *m_selectWheelIm));
			m_songWheelText.push_back(std::make_unique<Text>(glm::vec2(865, 637+yPos), L"^g[...^^^", 415, 24, 1));
			m_buttons.at(i)->readjustBounds(glm::vec2(829, 630+yPos));
			yPos -= 57;
		}

		glm::mat4 model{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_shader->use();
		m_shader->setMat4("model", model);
		m_shader->setMat4("projection", projection);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");

		m_textShader->use();
		m_textShader->setMat4("model", model);
		m_textShader->setMat4("projection", projection);
	}
	void SongScene::Update(float dt)
	{
		#pragma region song wheel scrolling
		//Checking if scroll reached 57 pixels up or down
		if (m_pixels >= 57)
		{
			m_scrollDirection = 0;
			m_pixels = 0;
		}

		//Scrolling the song wheel up or down by 57 pixels (height of a song wheel part)
		m_slow += dt;
		if(m_scrollDirection == 1 && m_slow > 0.001)
		{
			m_slow = 0;
			m_pixels += 1;
			for(int i = 0; i < m_buttonPositions.size(); i++)
			{
				m_buttonPositions.at(i) += 1;
				m_buttons.at(i)->repositionVerts(glm::vec2(829, m_buttonPositions.at(i)));
				m_buttons.at(i)->readjustBounds(glm::vec2(829, m_buttonPositions.at(i)));
				m_songWheelText.at(i)->repositionVerts(glm::vec2(865, m_buttonPositions.at(i) + 7));
			}
		}
		else if(m_scrollDirection == -1 && m_slow > 0.001)
		{
			m_slow = 0;
			m_pixels += 1;
			for (int i = 0; i < m_buttonPositions.size(); i++)
			{
				m_buttonPositions.at(i) -= 1;
				m_buttons.at(i)->repositionVerts(glm::vec2(829, m_buttonPositions.at(i)));
				m_buttons.at(i)->readjustBounds(glm::vec2(829, m_buttonPositions.at(i)));
				m_songWheelText.at(i)->repositionVerts(glm::vec2(865, m_buttonPositions.at(i) + 7));
			}
		}

		//Checking if a part of the song wheel gone past the upper limit or lower limit
		for(int i = 0; i < m_buttonPositions.size(); i++)
		{
			if(m_buttonPositions.at(i) > m_upperLimit)
			{
				m_buttonPositions.at(i) = 3;
			}
			else if(m_buttonPositions.at(i) < m_lowerLimit)
			{
				m_buttonPositions.at(i) = 686;
			}
		}
		#pragma endregion

	}
	void SongScene::Render()
	{
		glClearColor(0.5568f, 0.8f, 0.7764f, 0.f);

		for(int i = 0; i < 12; i++)
		{
			m_buttons.at(i)->Draw(*m_shader);
			m_songWheelText.at(i)->Draw(*m_textShader);
		}
		
		m_shader->use();
		m_shader->setMat4("model", glm::mat4{ 1.0f });
		m_songBG->Draw(*m_shader);
		m_songSelect->Draw(*m_shader);
		m_logo->Draw(*m_shader);

		m_textShader->use();
		m_textShader->setMat4("model", glm::mat4{ 1.0f });
	}
	void SongScene::getKey(int key, int scancode, int action, int mods)
	{
		if(action == GLFW_PRESS && key == GLFW_KEY_F5)
		{
			m_fileProcessor->ProcessFiles();
			m_fileProcessor->reloadSongs();
		}
		if(action == GLFW_PRESS && key == GLFW_KEY_F6)
		{
			std::wcout << m_fileProcessor->retrieveSong(0).artist << "\n";
			std::wcout << m_fileProcessor->retrieveSong(0).title << "\n";
			std::wcout << m_fileProcessor->retrieveSong(0).dirPath << "\n";
			std::wcout << m_fileProcessor->retrieveSong(0).osuPath << "\n";
			std::wcout << m_fileProcessor->retrieveSong(0).background << "\n";
			std::wcout << m_fileProcessor->retrieveSong(0).audio << "\n";
		}
		if(action == GLFW_PRESS && key == GLFW_KEY_SPACE)
		{
			m_songWheelText.at(0)->changeText(L"click");
		}
	}
	void SongScene::getMouseButton(int button, int action, int mods)
	{
		double posX, posY;
		glfwGetCursorPos(m_window, &posX, &posY);
		if(m_logo->hasMouseOnTop(posX,posY) && action == GLFW_PRESS)
		{
			std::cout << m_logo->getZIndex() << "\n";
		}
		
		if(m_buttons.at(0)->hasMouseOnTop(posX, posY) && action == GLFW_PRESS)
		{
			std::cout << "a" << "\n";
		}
		
		if(m_buttons.at(1)->hasMouseOnTop(posX, posY) && action == GLFW_PRESS)
		{
			std::cout << "b" << "\n";
		}
	}
	void SongScene::getScroll(double xoffset, double yoffset)
	{
		//Lowest song wheel is at -627 relative to the song wheel part
		//std::cout << yoffset << "\n";
		if(m_scrollDirection == 0)
		{
			m_scrollDirection = yoffset;
			{
				m_SFXscroll = std::make_unique<SFXSource>();
				m_SFXscroll->Play(m_scrollSound);
			}
		}
	}
	void SongScene::fileDrop(int count, const char** paths)
	{

	}
}

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
		m_fileProcessor = std::make_unique<FileProcessor>();

		m_SFXscroll = std::make_unique<SFXSource>();
		m_SFXstart = std::make_unique<SFXSource>();

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
			m_buttonPositions.push_back(yPos);
			m_buttons.push_back(std::make_unique<Button>(glm::vec2(829, 630), glm::vec2(451, 57), 0, *m_selectWheelIm));
			m_songWheelText.push_back(std::make_unique<Text>(glm::vec2(865, 645), L"...", 451, 32, 1));
			m_buttons.at(i)->readjustBounds(glm::vec2(829, 630+yPos));
			yPos -= 57;
		}

		glm::mat4 model{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_shader->use();
		m_shader->setMat4("model", model);
		m_shader->setMat4("projection", projection);

		m_text = std::make_unique<Text>(glm::vec2(5,690), L"hello あ",200 ,32, 3);
		m_text2 = std::make_unique<Text>(glm::vec2(5,390), L"...",100 ,32, 3);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");

		m_textShader->use();
		m_textShader->setMat4("model", model);
		m_textShader->setMat4("projection", projection);
	}
	void SongScene::Update(float dt)
	{

	}
	void SongScene::Render()
	{
		glClearColor(0.5568f, 0.8f, 0.7764f, 0.f);

		for(int i = 0; i < 12; i++)
		{
			m_buttons.at(i)->Draw(*m_shader);
			m_shader->setMat4("model", glm::translate(glm::mat4{1.0f}, glm::vec3(0, m_buttonPositions.at(i), 0)));
		}
		m_shader->setMat4("model", glm::mat4{1.0f});

		for(int i = 0; i < 12; i++)
		{
			m_songWheelText.at(i)->Draw(*m_textShader);
			m_textShader->setMat4("model", glm::translate(glm::mat4{ 1.0f }, glm::vec3(0, m_buttonPositions.at(i), 0)));
		}

		m_shader->setMat4("model", glm::mat4{ 1.0f });
		m_songBG->Draw(*m_shader);
		m_songSelect->Draw(*m_shader);
		m_logo->Draw(*m_shader);

		m_textShader->setMat4("model", glm::mat4{ 1.0f });
		m_text->Draw(*m_textShader);
		m_text2->Draw(*m_textShader);
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
			m_songWheelText.at(2)->changeText(L"a");
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
		std::cout << yoffset << "\n";
	}
	void SongScene::fileDrop(int count, const char** paths)
	{

	}
}

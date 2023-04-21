/*****************************************************************//**
 * \file   SongScene.cpp
 * \brief  Song select scene implementation
 * 
 * \date   February 2023
 *********************************************************************/
#include "Scenes/SongScene.h"
#include "Utils/Properties.h"
#include "Utils/Settings.h"
#include <GLFW/glfw3.h>
#include <future>

namespace SJ
{
	SongScene::SongScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		m_fileProcessor = std::make_unique<FileProcessor>();

	#pragma region Main graphics and audio
		m_scrollSound = m_sfx->addSFX(SJFOLDER + SOUNDS + "scroll.wav");
		m_refreshSound = m_sfx->addSFX(SJFOLDER + SOUNDS + "refresh.wav");
		m_startSound = m_sfx->addSFX(SJFOLDER + SOUNDS + "begin.wav");

		m_songSelectIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "songselectmenu.png", GL_CLAMP_TO_EDGE);
		m_songSelect = std::make_unique<Rect>(glm::vec2(0.f,0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 2, *m_songSelectIm);

		m_logoIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_logoBtn = std::make_unique<Button>(glm::vec2(0.f, 0.f), glm::vec2(125.f, 125.f), 3, *m_logoIm);

		m_songBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "selectbg.png", GL_CLAMP_TO_EDGE);
		m_songBg = std::make_unique<Rect>(glm::vec2(0.f, 0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_songBGIm);

		m_songBGImPlay = std::make_unique<Texture>("", GL_CLAMP_TO_EDGE);
		m_songBGPlay = std::make_unique<Rect>(glm::vec2(0.0f, 0.0f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 10, *m_songBGImPlay);

		m_selectWheelIm = std::make_shared<Texture>(SJFOLDER + IMAGES + "selectbar.png", GL_CLAMP_TO_EDGE);

		int yPos = 0;
		for (int i = 0; i < 11; i++)
		{
			m_buttonPositions.push_back(630+yPos);
			m_buttons.push_back(std::make_unique<Button>(glm::vec2(829, 630+yPos), glm::vec2(451, 57), 0, *m_selectWheelIm));
			m_songWheelText.push_back(std::make_unique<Text>(glm::vec2(865, 637+yPos), L"...", 415, 24, 1, "NotoSansJP-Light.otf"));
			m_buttons.at(i)->readjustBounds(glm::vec2(829, 630+yPos));
			yPos -= 57;
		}

		m_songText = std::make_unique<Text>(glm::vec2(25, 380), L" ", 350, 96, 5, "NotoSansJP-Regular.otf");
		m_artistText = std::make_unique<Text>(glm::vec2(25, 500), L" ", 350, 48, 5, "NotoSansJP-Medium.otf");
		m_diffText = std::make_unique<Text>(glm::vec2(25, 350), L" ", 350, 36, 5, "NotoSansJP-Regular.otf");

		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->setMat4("projection", projection);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");
		m_textShader->setMat4("projection", projection);
	#pragma endregion

	#pragma region settings UI

	#pragma endregion

	#pragma region exit UI
		m_exitBgIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "exitbg.png", GL_CLAMP_TO_EDGE);
		m_exitBg = std::make_unique<Rect>(glm::vec2(500.f,300.f), glm::vec2(300.f, 200.f), 6, *m_exitBgIm);

		m_exitYesIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "yes.png", GL_CLAMP_TO_EDGE);
		m_exitNoIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "no.png", GL_CLAMP_TO_EDGE);
		m_exitYesBtn = std::make_unique<Button>(glm::vec2(515.f, 315.f), glm::vec2(121.f, 51.f), 7, *m_exitYesIm);
		m_exitNoBtn = std::make_unique<Button>(glm::vec2(663.f, 315.f), glm::vec2(121.f, 51.f), 7, *m_exitNoIm);
	#pragma endregion

	#pragma region song data processing
		if(g_filesChanged || !std::filesystem::exists(L"../ShuiJamGame/shuijam.db"))
		{
			m_fileProcessor->ProcessFiles();
			m_fileProcessor->reloadSongs();
		}
		m_lastSong = m_fileProcessor->getLastID();
		updateSongWheel();
	#pragma endregion
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

		//Checking if a part of the song wheel gone past the upper limit or lower limit
		//The higher number the higher up on the screen
		for (int i = 0; i < m_buttonPositions.size(); i++)
		{
			if (m_buttonPositions.at(i) + 57 >= m_upperLimit && m_scrollDirection == 1)
			{
				m_buttonPositions.at(i) = 3;
			}
			else if (m_buttonPositions.at(i) <= m_lowerLimit && m_scrollDirection == -1)
			{
				m_buttonPositions.at(i) = 687;
			}
		}

		//Scrolling the song wheel up or down by 57 pixels (height of a song wheel part)
		m_slow += dt;
		if(m_scrollDirection == 1 && m_slow > 0.0005)
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
		else if(m_scrollDirection == -1 && m_slow > 0.0005)
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

		#pragma endregion

		if(m_music != nullptr && m_isPlaying)
		{
			m_music->Play();
			m_music->Update();
		}
	}
	void SongScene::Render()
	{
		double posX, posY;
		glfwGetCursorPos(m_window, &posX, &posY);

		for(int i = 0; i < m_buttonPositions.size(); i++)
		{
			//Highlight the cursor hovering over the wheel
			//Highlight the selected song wheel
			if (m_confirmation == i) { m_shader->setFloat("transparency", 1.2f); }
			else if(m_buttons.at(i)->hasMouseOnTop(posX,posY)) { m_shader->setFloat("transparency", 1.1f); }
			else {m_shader->setFloat("transparency", 1.f); }
			m_buttons.at(i)->Draw(*m_shader);
			m_songWheelText.at(i)->Draw(*m_textShader);
		}

		m_shader->setFloat("transparency", 0.7f);
		m_songBg->Draw(*m_shader);
		m_shader->setFloat("transparency", 1.0f);
		m_songSelect->Draw(*m_shader);

		if (m_logoBtn->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
		else m_shader->setFloat("transparency", 1.0f);
		m_logoBtn->Draw(*m_shader);
		m_shader->setFloat("transparency", 1.0f);

		//Large text on the left that displays the full detail of the song name and difficulty
		m_songText->Draw(*m_textShader);
		m_artistText->Draw(*m_textShader);
		m_diffText->Draw(*m_textShader);

		//Draw the exit graphics if the player has clicked the logo
		if(m_exitOpen)
		{
			m_exitBg->Draw(*m_shader);
			m_exitNoBtn->Draw(*m_shader);
			m_exitYesBtn->Draw(*m_shader);
		}
	}
	void SongScene::getKey(int key, int scancode, int action, int mods)
	{
		//Refresh the song list when the player presses F5
		if(action == GLFW_PRESS && key == GLFW_KEY_F5)
		{
			{
				m_source = std::make_unique<SFXSource>();
				m_source->Play(m_refreshSound);
			}
			m_fileProcessor->ProcessFiles();
			m_fileProcessor->reloadSongs();
			m_lastSong = m_fileProcessor->getLastID();
			updateSongWheel();
		}
		//An alternative way to scroll up or down the song wheel
		if((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_UP && m_scrollDirection == 0 && m_canScrollDown)
		{
			scrollDown();
			updateSongWheel();
		}
		else if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_DOWN && m_scrollDirection == 0 && m_canScrollUp)
		{
			scrollUp();
			updateSongWheel();
		}
		else if(action == GLFW_PRESS && key == GLFW_KEY_ENTER)
		{
			if(m_confirmation != -1)
			{
				startGame();
			}
		}
	}
	void SongScene::getMouseButton(int button, int action, int mods)
	{
		if (!m_canClick) return;
	#pragma region exit buttons
		double posX, posY;
		glfwGetCursorPos(m_window, &posX, &posY);
		if(m_logoBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS)
		{
			if(!m_exitOpen)
			{
				m_source = std::make_unique<SFXSource>();
				m_source->Play(m_refreshSound);
			}
			m_exitOpen = true;
		}
		else if(m_exitOpen == true)
		{
			if(m_exitYesBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(m_window, true);
			}
			else if(m_exitNoBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS)
			{
				m_exitOpen = false;
			}
		}
	#pragma endregion

	#pragma region songwheel buttons
		for(int i = 0; i < m_buttons.size(); i++)
		{
			if (m_buttons.at(i)->hasMouseOnTop(posX, posY) && action == GLFW_PRESS) 
			{
				if (m_confirmation != i) 
				{ 
					m_songText->changeText(m_songData.at(i).title.substr(0, m_songData.at(i).title.find_first_of(L"[")));
					m_artistText->changeText(m_songData.at(i).artist);
					m_diffText->changeText(m_songData.at(i).version);
					m_confirmation = i; 
					{
						m_source = std::make_unique<SFXSource>();
						m_source->Play(m_scrollSound);
						if (m_music != nullptr) m_music.reset();
						m_music = std::make_unique<Music>(L"../ShuiJamGame/Songs/" + m_songData.at(i).dirPath + L"/" + m_songData.at(i).audio);
						m_songBGIm->reloadTexture(L"../ShuiJamGame/Songs/" + m_songData.at(i).dirPath + L"/" + m_songData.at(i).background, GL_CLAMP_TO_EDGE);
						m_songBGImPlay->reloadTexture(L"../ShuiJamGame/Songs/" + m_songData.at(i).dirPath + L"/" + m_songData.at(i).background, GL_CLAMP_TO_EDGE);
						m_isPlaying = true;
					}
				}
				else if (m_confirmation == i)
				{
					startGame();
				}
			}
		}
	#pragma endregion
	}
	void SongScene::getScroll(double xoffset, double yoffset)
	{
		if(m_scrollDirection == 0)
		{	
			if(yoffset == 1 && m_canScrollDown)
			{
				scrollDown();
			}
			else if(yoffset == -1 && m_canScrollUp)
			{
				scrollUp();
			}
			updateSongWheel();
		}
	}
	void SongScene::fileDrop(int count, const char** paths)
	{

	}

	void SongScene::updateSongWheel()
	{
		//A walker that goes through each empty song data in the array and fills it with song data
		//Ptr goes back to zero as a way to wrap around if there isnt enough song data within the database
		//The text changes based on the data inside m_songData
		if (m_head <= 0) 
		{ 
			m_canScrollDown = true;
			m_canScrollUp = false;
		}
		else m_canScrollUp = true;
		if (m_tail > m_lastSong)
		{
			m_canScrollDown = false;
			m_canScrollUp = true;
		}
		else m_canScrollDown = true;

		int ptr = m_head;
		for (int i = 0; i < 11; i++)
		{
			if (ptr > m_lastSong) ptr = 0;
			int index = (m_head + i) % 11;
			m_songData.at(index) = m_fileProcessor->retrieveSong(ptr);
			//std::wcout << index << ": " << m_songData.at(index).title << "\n";
			m_songWheelText.at(index)->changeText(m_fileProcessor->retrieveSong(ptr).title);
			ptr++;
			if (m_songData.at(index).title.size() > 30)
			{
				m_songWheelText.at(index)->changeText(m_songData.at(index).title.substr(0, 30) + L"...");
			}
			else
			{
				m_songWheelText.at(index)->changeText(m_songData.at(index).title);
			}
		}
	}

	void SongScene::startGame()
	{
		//Swap scenes here
		m_canClick = false;
		g_CurrentBG = m_songData.at(m_confirmation).background;
		g_CurrentSong = m_songData.at(m_confirmation).audio;
		g_CurrentDifficulty = m_songData.at(m_confirmation).osuPath;
		g_CurrentOsuDir = m_songData.at(m_confirmation).dirPath;
		{
			m_source = std::make_unique<SFXSource>();
			m_source->Play(m_startSound);
			m_music.reset();
		}
		g_CurrentScene = "game";
	}

	void SongScene::scrollDown()
	{
		m_head++;//increment the head position
		m_tail++;//increment the tail position
		m_confirmation = -1;
		m_scrollDirection = 1;
		{
			m_source = std::make_unique<SFXSource>();
			m_source->Play(m_scrollSound);
			if (m_music != nullptr) 
			{ 
				m_isPlaying = false;
				m_music.reset();
				m_songBGIm->reloadTexture(SJFOLDER + IMAGES + "selectbg.png", GL_CLAMP_TO_EDGE);
			}
		}
		m_songText->changeText(L" ");
		m_artistText->changeText(L" ");
		m_diffText->changeText(L" ");
	}

	void SongScene::scrollUp()
	{
		m_head--;//decrement the head position
		m_tail--;//decrement the tail position
		m_confirmation = -1;
		m_scrollDirection = -1;
		{
			m_source = std::make_unique<SFXSource>();
			m_source->Play(m_scrollSound);
			if (m_music != nullptr) 
			{
				m_isPlaying = false;
				m_music.reset();
				m_songBGIm->reloadTexture(SJFOLDER + IMAGES + "selectbg.png", GL_CLAMP_TO_EDGE);
			}
		}
		m_songText->changeText(L" ");
		m_artistText->changeText(L" ");
		m_diffText->changeText(L" ");
	}
}

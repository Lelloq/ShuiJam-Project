/*****************************************************************//**
 * \file   SongScene.cpp
 * \brief  Song select scene implementation
 * 
 * \date   February 2023
 *********************************************************************/
#include "Scenes/SongScene.h"
#include "Utils/Properties.h"
#include "Utils/SettingsManager.h"
#include <GLFW/glfw3.h>
#include <future>

namespace SJ
{
	SongScene::SongScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		SettingsManager::Load();
		m_fileProcessor = std::make_unique<FileProcessor>();
		g_failed = false;

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

		m_songText = std::make_unique<Text>(glm::vec2(25, 380), L" ", 350, 96, 2, "NotoSansJP-Regular.otf");
		m_artistText = std::make_unique<Text>(glm::vec2(25, 500), L" ", 350, 48, 2, "NotoSansJP-Medium.otf");
		m_diffText = std::make_unique<Text>(glm::vec2(25, 350), L" ", 350, 36, 2, "NotoSansJP-Regular.otf");

		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->setMat4("projection", projection);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");
		m_textShader->setMat4("projection", projection);
	#pragma endregion

	#pragma region settings UI
		m_settingsIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "settingsicon.png", GL_CLAMP_TO_EDGE);
		m_settingsBtn = std::make_unique<Button>(glm::vec2(200.f, 5.f), glm::vec2(64.f,64.f), 3, *m_settingsIm);

		m_settingsBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "settingsbg.png", GL_CLAMP_TO_EDGE);
		m_settingsBG = std::make_unique<Rect>(glm::vec2((VPORT_WIDTH / 2) - (m_settingsBGIm->getWidthf() / 2),
			(VPORT_HEIGHT / 2) - (m_settingsBGIm->getHeightf() / 2)), glm::vec2(m_settingsBGIm->getWidthf(), m_settingsBGIm->getHeightf()), 3, * m_settingsBGIm);

		m_offsetText = std::make_unique<Text>(glm::vec2(0,0), L"Offset:", 300, 24, 4, "NotoSansJP-Regular.otf");
		m_offsetText->repositionVerts(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 72));
		m_leftButtonIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "buttonleft.png", GL_CLAMP_TO_EDGE);
		m_rightButtonIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "buttonright.png", GL_CLAMP_TO_EDGE);
		m_offsetButtonL = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 100),
			glm::vec2(m_leftButtonIm->getWidthf(), m_leftButtonIm->getHeightf()), 4, *m_leftButtonIm);
		m_offsetButtonR = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40 + m_leftButtonIm->getWidthf(), m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 100),
			glm::vec2(m_rightButtonIm->getWidthf(), m_rightButtonIm->getHeightf()), 4, *m_rightButtonIm);
		m_offsetNum = std::make_unique<Text>(
			glm::vec2(m_offsetButtonR->getPosition().x + 50, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 107), L": " + std::to_wstring(g_offset), 300, 28, 5, "NotoSansJP-Regular.otf");

		m_hitposText = std::make_unique<Text>(glm::vec2(0, 0), L"Hitposition:", 300, 24, 4, "NotoSansJP-Regular.otf");
		m_hitposText->repositionVerts(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 148));
		m_hitposButtonL = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 176),
			glm::vec2(m_leftButtonIm->getWidthf(), m_leftButtonIm->getHeightf()), 4, *m_leftButtonIm);
		m_hitposButtonR = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40 + m_leftButtonIm->getWidthf(), m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 176),
			glm::vec2(m_rightButtonIm->getWidthf(), m_rightButtonIm->getHeightf()), 4, *m_rightButtonIm);
		m_hitposNum = std::make_unique<Text>(
			glm::vec2(m_offsetButtonR->getPosition().x + 50, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 183), L": " + std::to_wstring(g_hitposition), 300, 28, 5, "NotoSansJP-Regular.otf");

		m_volumeText = std::make_unique<Text>(glm::vec2(0, 0), L"Volume:", 300, 24, 4, "NotoSansJP-Regular.otf");
		m_volumeText->repositionVerts(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 220));
		m_volumeButtonL = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 248),
			glm::vec2(m_leftButtonIm->getWidthf(), m_leftButtonIm->getHeightf()), 4, *m_leftButtonIm);
		m_volumeButtonR = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40 + m_leftButtonIm->getWidthf(), m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 248),
			glm::vec2(m_rightButtonIm->getWidthf(), m_rightButtonIm->getHeightf()), 4, *m_rightButtonIm);
		std::wstring volStr = std::to_wstring(g_volume).substr(0, std::to_wstring(g_volume).find_first_of('.') + 3);
		m_volumeNum = std::make_unique<Text>(
			glm::vec2(m_offsetButtonR->getPosition().x + 50, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 255), L": " + volStr, 300, 28, 5, "NotoSansJP-Regular.otf");

		m_speedText = std::make_unique<Text>(glm::vec2(0, 0), L"Scroll Speed:", 300, 24, 4, "NotoSansJP-Regular.otf");
		m_speedText->repositionVerts(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 292));
		m_speedButtonL = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 320),
			glm::vec2(m_leftButtonIm->getWidthf(), m_leftButtonIm->getHeightf()), 4, *m_leftButtonIm);
		m_speedButtonR = std::make_unique<Button>(glm::vec2(m_settingsBG->getPosition().x + 40 + m_leftButtonIm->getWidthf(), m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 320),
			glm::vec2(m_rightButtonIm->getWidthf(), m_rightButtonIm->getHeightf()), 4, *m_rightButtonIm);
		m_speedNum = std::make_unique<Text>(
			glm::vec2(m_offsetButtonR->getPosition().x + 50, m_settingsBG->getPosition().y + m_settingsBG->getSize().y - 327), L": " + std::to_wstring(g_scrollspeed), 300, 28, 5, "NotoSansJP-Regular.otf");
	#pragma endregion

	#pragma region keybinds UI
		m_keybindIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "keybinds.png", GL_CLAMP_TO_EDGE);
		m_keybindBtn = std::make_unique<Button>(glm::vec2(328.f, 5.f), glm::vec2(64.f, 64.f), 3, *m_keybindIm);

		m_keybindBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "keybindbg.png", GL_CLAMP_TO_EDGE);
		m_keybindBG = std::make_unique<Rect>(glm::vec2((VPORT_WIDTH / 2) - (m_keybindBGIm->getWidthf() / 2),
			(VPORT_HEIGHT / 2) - (m_keybindBGIm->getHeightf() / 2)), glm::vec2(m_keybindBGIm->getWidthf(), m_keybindBGIm->getHeightf()), 3, *m_keybindBGIm);

		m_keyBtnIm = std::make_shared<Texture>(SJFOLDER + IMAGES + "keybutton.png", GL_CLAMP_TO_EDGE);

		float xPos = 433.f;
		for(int i = 0; i < 7; i++)
		{
			m_keyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/key" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_key.at(i) = std::make_unique<Rect>(glm::vec2(xPos, 365.f),glm::vec2(59.f, 79.f), 4, *m_keyIm.at(i));
			m_keyBtn.at(i) = std::make_unique<Button>(glm::vec2(xPos, 280.f),glm::vec2(59.f, 79.f), 4, *m_keyBtnIm);
			m_keyTxt.at(i) = std::make_unique<Text>(glm::vec2(xPos+12, 290.f), std::to_wstring(m_inputs.at(i)), 59.f, 32, 5, "NotoSansJP-Medium.otf");
			xPos += 59.f;
		}


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
		if(g_filesChanged)
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

		if (m_settingsBtn->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
		else m_shader->setFloat("transparency", 1.0f);
		m_settingsBtn->Draw(*m_shader);
		m_shader->setFloat("transparency", 1.0f);

	#pragma region settings
		if(m_settingsOpen)
		{
			m_settingsBG->Draw(*m_shader);

			m_offsetText->Draw(*m_textShader);
			if (m_offsetButtonL->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_offsetButtonL->Draw(*m_shader);
			if (m_offsetButtonR->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_offsetButtonR->Draw(*m_shader);
			m_offsetNum->Draw(*m_textShader);

			m_hitposText->Draw(*m_textShader);
			if (m_hitposButtonL->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_hitposButtonL->Draw(*m_shader);
			if (m_hitposButtonR->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_hitposButtonR->Draw(*m_shader);
			m_hitposNum->Draw(*m_textShader);

			m_volumeText->Draw(*m_textShader);
			if (m_volumeButtonL->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_volumeButtonL->Draw(*m_shader);
			if (m_volumeButtonR->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_volumeButtonR->Draw(*m_shader);
			m_volumeNum->Draw(*m_textShader);
			
			m_speedText->Draw(*m_textShader);
			if (m_speedButtonL->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_speedButtonL->Draw(*m_shader);
			if (m_speedButtonR->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
			else m_shader->setFloat("transparency", 1.0f);
			m_speedButtonR->Draw(*m_shader);
			m_speedNum->Draw(*m_textShader);
		}

		if (m_keybindBtn->hasMouseOnTop(posX, posY)) { m_shader->setFloat("transparency", 2.0f); }
		else m_shader->setFloat("transparency", 1.0f);
		m_keybindBtn->Draw(*m_shader);
		m_shader->setFloat("transparency", 1.0f);

		if(m_keybindsOpen)
		{
			m_keybindBG->Draw(*m_shader);
			for(int i = 0; i < 7; i++)
			{
				if(m_pressed.at(i) && !m_changing)
				{
					m_key.at(i)->Draw(*m_shader);
				}
				m_keyBtn.at(i)->Draw(*m_shader);
				m_keyTxt.at(i)->Draw(*m_textShader);
			}
		}
	#pragma endregion

	#pragma region keybinds

	#pragma endregion
	}
	void SongScene::getKey(int key, int scancode, int action, int mods)
	{
	#pragma region main song buttons
		//Refresh the song list when the player presses F5
		if(action == GLFW_PRESS && key == GLFW_KEY_F5)
		{
			playRefresh();
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
	#pragma endregion
		//For settings to change the increment of the settings values
		if(action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT) { m_shiftHeld = true; }
		else if (action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT) { m_shiftHeld = false; }
	#pragma region keybind tester and changer
		//Tester
		if(!m_changing)
		{
			for(int i = 0; i < 7;i++)
			{
				if (action == GLFW_PRESS && key == m_inputs.at(i))
				{
					m_pressed.at(i) = true;
				}
				else if (action == GLFW_RELEASE && key == m_inputs.at(i))
				{
					m_pressed.at(i) = false;
				}
			}
		}
		else if(m_changing)
		{
			for(int i = 0; i < 7; i++)
			{
				if(m_keyForChange.at(i) == true)
				{
					m_inputs.at(i) = key;
					m_changing = false;
					m_keyForChange.at(i) = false;
					m_keyTxt.at(i)->changeText(std::to_wstring(m_inputs.at(i)));
				}
			}
			g_keyOne = m_inputs.at(0);
			g_keyTwo = m_inputs.at(1);
			g_keyThree = m_inputs.at(2);
			g_keyFour = m_inputs.at(3);
			g_keyFive = m_inputs.at(4);
			g_keySix = m_inputs.at(5);
			g_keySeven = m_inputs.at(6);
			SettingsManager::Save();
		}
	#pragma endregion
	}
	void SongScene::getMouseButton(int button, int action, int mods)
	{
		if (!m_canClick) return;
		double posX, posY;
		glfwGetCursorPos(m_window, &posX, &posY);

	#pragma region exit buttons
		if(m_logoBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
		{
			if(!m_exitOpen)
			{
				playRefresh();
				m_exitOpen = true;
			}
			m_settingsOpen = false;
			m_keybindsOpen = false;
		}
		else if(m_exitOpen == true)
		{
			if(m_exitYesBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				glfwSetWindowShouldClose(m_window, true);
			}
			else if(m_exitNoBtn->hasMouseOnTop(posX,posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				m_exitOpen = false;
			}
		}
	#pragma endregion

	#pragma region setting buttons
		if (m_settingsBtn->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
		{
			if (!m_settingsOpen) 
			{ 
				playRefresh();
				m_settingsOpen = true; 
				m_exitOpen = false;
				m_keybindsOpen = false;
			}
			else { m_settingsOpen = false; }
		}
		else if(m_settingsOpen)
		{
			if(m_offsetButtonL->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_offset -= 10;
				else g_offset--;
			}
			else if (m_offsetButtonR->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_offset += 10;
				else g_offset++;
			}
			g_offset = glm::clamp(g_offset, -300, 300);
			m_offsetNum->changeText(L": "+ std::to_wstring(g_offset));

			if (m_hitposButtonL->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_hitposition -= 10;
				else g_hitposition--;
			}
			else if (m_hitposButtonR->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_hitposition += 10;
				else g_hitposition++;
			}
			g_hitposition = glm::clamp(g_hitposition, 0, 360);
			m_hitposNum->changeText(L": " + std::to_wstring(g_hitposition));

			if (m_volumeButtonL->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_volume -= 0.1f;
				else g_volume -= 0.01f;
			}
			else if (m_volumeButtonR->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_volume += 0.1f;
				else g_volume += 0.01f;
			}
			g_volume = glm::clamp(g_volume, 0.0f, 1.0f);
			std::wstring volStr = std::to_wstring(g_volume).substr(0, std::to_wstring(g_volume).find_first_of('.') + 3);
			m_volumeNum->changeText(L": " + volStr);
			m_device->setGain(g_volume);

			if (m_speedButtonL->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_scrollspeed -= 10;
				else g_scrollspeed--;
			}
			else if (m_speedButtonR->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
			{
				if (m_shiftHeld) g_scrollspeed += 10;
				else g_scrollspeed++;
			}
			g_scrollspeed = glm::clamp(g_scrollspeed, 250, 1500);
			m_speedNum->changeText(L": " + std::to_wstring(g_scrollspeed));
			SettingsManager::Save();
		}
	#pragma endregion

	#pragma region keybind
		if (m_keybindBtn->hasMouseOnTop(posX, posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
		{
			if (!m_keybindsOpen)
			{
				playRefresh();
				m_settingsOpen = false;
				m_exitOpen = false;
				m_keybindsOpen = true;
			}
			else m_keybindsOpen = false;
		}
		else if(m_keybindsOpen)
		{
			for(int i = 0; i < 7; i++)
			{
				if(m_keyBtn.at(i)->hasMouseOnTop(posX,posY) && action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
				{
					m_changing = true;
					m_keyForChange.at(i) = true;
					m_keyTxt.at(i)->changeText(L" ");
				}
			}
		}
	#pragma endregion

	#pragma region songwheel buttons
		//Keybind screen overlaps it slightly so I disabled it while it is open
		if(!m_keybindsOpen)
		{
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
			m_head = 0;
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
			ptr++;
			if (m_songData.at(index).title.size() + m_songData.at(index).version.size() > 30)
			{
				m_songWheelText.at(index)->changeText(m_songData.at(index).title.substr(0, 30) + L"...");
			}
			else
			{
				m_songWheelText.at(index)->changeText(m_songData.at(index).title + L" [" + m_songData.at(index).version + L"]");
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
		g_CurrentDiffName = m_songData.at(m_confirmation).version;
		g_CurrentTitle = m_songData.at(m_confirmation).title;
		{
			playStart();
			m_music.reset();
		}
		g_CurrentScene = "game";
		m_canClick = true;
	}

	void SongScene::scrollDown()
	{
		m_head++;//increment the head position
		m_tail++;//increment the tail position
		m_confirmation = -1;//Deselect the current song selected
		m_scrollDirection = 1;
		{
			playScroll();
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
			playScroll();
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

	void SongScene::playRefresh() 
	{
		m_source = std::make_unique<SFXSource>();
		m_source->Play(m_refreshSound);
	}
	
	void SongScene::playScroll() 
	{
		m_source = std::make_unique<SFXSource>();
		m_source->Play(m_scrollSound);
	}
	
	void SongScene::playStart() 
	{
		m_source = std::make_unique<SFXSource>();
		m_source->Play(m_startSound);
	}
}

/*****************************************************************//**
 * \file   GameScene.cpp
 * \date   March 2023
 *********************************************************************/
#include "Scenes/GameScene.h"
#include "Utils/Properties.h"
#include <future>
#include <format>

namespace SJ
{
	GameScene::GameScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
			m_riceIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/note" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_headIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/head" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_tailIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/tail" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			m_bodyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "notes/body" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
			//Key press images
			m_keyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/key" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
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
		m_stageHitposition = std::make_unique<Rect>(glm::vec2(middleBL, m_hitPosition), glm::vec2(m_stageHitpositionIm->getWidth(), m_stageHitpositionIm->getHeight()), 2, *m_stageHitpositionIm);
		//Health
		m_health = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_stageRightIm->getWidth(), 0), glm::vec2(m_healthIm->getWidth(), m_healthIm->getHeight()), 2, *m_healthIm);
		m_healthBG = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_stageRightIm->getWidth(), 0), glm::vec2(m_healthBGIm->getWidth(), m_healthBGIm->getHeight()), 1, *m_healthBGIm);
		//Key
		int xPos = middleBL;
		for(int i = 0; i < 7; i++)
		{
			m_key.at(i) = std::make_unique<Rect>(glm::vec2(xPos, 0), glm::vec2(m_stageBGIm->getWidth() / 7, m_keyIm.at(i)->getHeight()), 5, *m_keyIm.at(i));
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
				(glm::vec2(middleBL + (m_stageBGIm->getWidth() / 2) - m_judgementIm.at(i)->getWidth() / 2, m_judgePosition), 
				 glm::vec2(m_judgementIm.at(i)->getWidth(),m_judgementIm.at(i)->getHeight()), 3, *m_judgementIm.at(i));
		}

	#pragma endregion

	#pragma region Note creation
		for(int i = 0; i < 7; i++)
		{
			m_totalNotes += m_notes.at(i).size();
		}

		//int noteX = middleBL;
		//for(int i = 0; i < 7; i++)
		//{
		//	for(int j = 0; j < m_notes.at(i).size(); j++)
		//	{
		//		int release = m_notes.at(i).at(j).releasePoint;
		//		//m_timingBuffer converted to miliseconds for note position
		//		int timing = m_notes.at(i).at(j).timingPoint + m_leadin;
		//		if(release != 0)
		//		{
		//			m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, *m_headIm.at(i)));
		//			m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, release), 2, *m_bodyIm.at(i)));
		//			m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing + release), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, *m_tailIm.at(i)));
		//		}
		//		else
		//		{
		//			m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 2, *m_riceIm.at(i)));
		//		}
		//	}
		//	noteX += m_stageBGIm->getWidth() / 7;
		//}
	#pragma endregion
	}

	void GameScene::Update(float dt)
	{
		if(m_t1 < 1.f)
		{
			float time = lerp(m_leadin, 0, m_t1);
			m_t1 += (1.f / 2000.f) * 1000 * dt;
		}
		else
		{
			m_music->Play();
			m_music->Update();
			std::cout << m_music->getTimePosition() << "\n";
		}

		//Note spawning (Idea taken from https://www.gamedeveloper.com/programming/music-syncing-in-rhythm-games)
		//Originally for unity but the theory behind it can apply to here
		int noteX = (VPORT_WIDTH / 2) - (m_stageBGIm->getWidth() / 2);
		for (int i = 0; i < m_notes.size(); i++)
		{
			for(int j = m_nextNote.at(i); j < m_notes.at(i).size(); j++)
			{
				if(m_nextNote.at(i) < m_notes.at(i).size() && m_notes.at(i).at(m_nextNote.at(i)).timingPoint < m_music->getTimePosition() + 30000)
				{
					int release = m_notes.at(i).at(m_nextNote.at(i)).releasePoint;
					int timing = m_notes.at(i).at(m_nextNote.at(i)).timingPoint;
					if(release != 0)
					{
						m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, *m_headIm.at(i)));
						m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, release), 2, *m_bodyIm.at(i)));
						m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, *m_tailIm.at(i)));
					}
					else
					{
						m_noteObj.at(i).push_back(std::make_unique<Rect>(glm::vec2(noteX, timing), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 2, *m_riceIm.at(i)));
					}
					m_nextNote.at(i)++;
				}
			}
			noteX += m_stageBGIm->getWidth() / 7;
		}
		//Note moving
		noteX = (VPORT_WIDTH / 2) - (m_stageBGIm->getWidth() / 2);
		for(int i = 0; i < m_notes.size(); i++)
		{
			for (int j = 0; j < m_noteObj.at(i).size(); j++)
			{
				int timing = m_notes.at(i).at(j).timingPoint;
				int lerped = lerp(m_spawnPos, m_hitPosition,
					(550.0f - (timing - (m_music->getTimePosition()))) / 550.0f);
				m_noteObj.at(i).at(j)->repositionVerts(glm::vec2(noteX, lerped));
			}
			noteX += m_stageBGIm->getWidth() / 7;
		}
	}

	void GameScene::Render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	#pragma region Stage and numbers
		//Draw stage
		m_stageBG->Draw(*m_shader);
		m_stageLeft->Draw(*m_shader);
		m_stageRight->Draw(*m_shader);
		m_stageHitposition->Draw(*m_shader);
		//Draw health
		float healthPercent = m_hp / 100.f;
		m_health->resizeVerts(glm::vec2(m_healthIm->getWidth(), m_healthIm->getHeightf() * healthPercent));
		m_healthBG->Draw(*m_shader);
		m_health->Draw(*m_shader);
		//Draw accuracy
		m_percent->Draw(*m_shader);
		std::string m_accStr = std::to_string(m_accuracy);
		m_accStr = m_accStr.substr(0, m_accStr.find_first_of('.') + 3);
		int percentNumPosX = VPORT_WIDTH - m_percentIm->getWidth();
		int percentNumPosY = VPORT_HEIGHT - m_percentIm->getHeight();
		for(int i = m_accStr.size()-1; i >= 0; i--)
		{
			if(m_accStr.at(i) != '.')
			{
				percentNumPosX -= m_numIm.at(m_accStr.at(i) - '0')->getWidth();
				m_num.at(m_accStr.at(i) - '0')->repositionVerts(glm::vec2(percentNumPosX, percentNumPosY));
				m_num.at(m_accStr.at(i) - '0')->Draw(*m_shader);
			}
			else
			{
				percentNumPosX -= m_dotIm->getWidth();
				m_dot->repositionVerts(glm::vec2(percentNumPosX, percentNumPosY));
				m_dot->Draw(*m_shader);
			}
		}
		//Draw keys
		for(int i = 0; i < 7; i++)
		{
			if(m_pressed.at(i))
			{
				m_key.at(i)->Draw(*m_shader);
			}
		}
		if(m_hasHitRecently)
		{
			//Draw combo numbers
			std::string m_comboStr = std::to_string(m_combo);
			int comboPosX = VPORT_WIDTH / 2;
			if(m_combo != 0)
			{
				//Find the left most combo number position from the middle of the screen
				for(int i = 0; i < m_comboStr.size(); i++)
				{
					comboPosX -= m_numIm.at(m_comboStr.at(i) - '0')->getWidth() / 2;
				}
				//Draw the combo
				for(int i = 0; i < m_comboStr.size(); i++)
				{
					m_num.at(m_comboStr.at(i) - '0')->repositionVerts(glm::vec2(comboPosX, m_comboPosition));
					m_num.at(m_comboStr.at(i) - '0')->Draw(*m_shader);
					comboPosX += m_numIm.at(m_comboStr.at(i) - '0')->getWidth();
				}
			}
			//Draw judgement
			m_judgement.at(m_recentJudgement)->Draw(*m_shader);
		}
	#pragma endregion

	#pragma region Notes
		//Render notes as long as it is within view of the screen (need optimisation)
		//2D array as it accesses each column and goes through each note in that column
		for (int i = 0; i < m_noteObj.size(); i++)
		{
			for(auto& note : m_noteObj.at(i))
			{
				//Early exit to prevent any more accessing into notes that are outside of the viewport height
				if(note->getPosition().y > VPORT_HEIGHT) { break; }
				else if(note->getPosition().y <= VPORT_HEIGHT && note->getPosition().y + note->getSize().y >= 0)
				{
					note->Draw(*m_shader);
				}
				else if(note->getPosition().y + note->getSize().y > 0)
				{
					m_noteObj.at(i).erase(m_noteObj.at(i).begin());
				}
			}
		}
	#pragma endregion
	}

	void GameScene::getKey(int key, int scancode, int action, int mods)
	{
		for (int i = 0; i < 7; i++)
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

#pragma region NOT IN USE
	void GameScene::getMouseButton(int button, int action, int mods)
	{
	}

	void GameScene::getScroll(double xoffset, double yoffset)
	{
	}

	void GameScene::fileDrop(int count, const char** paths)
	{
	}
#pragma endregion

	float GameScene::lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}
}
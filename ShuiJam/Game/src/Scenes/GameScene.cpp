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
		//Disable cursor movement to prevent closing while the player is playing
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//Get all the note data
		m_notes = std::async(std::launch::async, OsuParser::parse, g_CurrentOsuDir, g_CurrentDifficulty).get();
		m_music = std::make_unique<Music>(m_folder + g_CurrentOsuDir + L"/" + g_CurrentSong);

		//Create shader
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->use();
		m_shader->setMat4("projection", projection);

		//Count total notes
		for(int i = 0; i < m_notes.size(); i++)
		{
			for(int j = 0; j < m_notes.at(i).size(); j++)
			{
				m_totalNotes++;
			}
		}

	#pragma region texture creation
		//Create textures
		for (int i = 0; i < 7; i++)
		{
			//Key press images
			m_keyIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/key" + std::to_string(i + 1) + ".png", GL_CLAMP_TO_EDGE);
		}
		//Stage images
		m_stageLeftIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stageleft.png", GL_CLAMP_TO_EDGE);
		m_stageRightIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stageright.png", GL_CLAMP_TO_EDGE);
		m_stageHitpositionIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stagehit.png", GL_CLAMP_TO_EDGE);
		m_stageBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stagebg.png", GL_CLAMP_TO_EDGE);
		m_stagebottomIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/stagebottom.png", GL_CLAMP_TO_EDGE);
		m_songBGIm = std::make_unique<Texture>(m_folder + g_CurrentOsuDir + L"/" + g_CurrentBG, GL_CLAMP_TO_EDGE);
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
		m_stageBG = std::make_unique<Rect>(glm::vec2(middleBL, 0),glm::vec2(m_stageBGIm->getWidth(),VPORT_HEIGHT), 1, *m_stageBGIm);
		m_stageLeft = std::make_unique<Rect>(glm::vec2(middleBL - m_stageLeftIm->getWidth(), 0), glm::vec2(m_stageLeftIm->getWidth(), VPORT_HEIGHT), 1, *m_stageLeftIm);
		m_stageRight= std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth(), 0), glm::vec2(m_stageRightIm->getWidth(), VPORT_HEIGHT), 1, *m_stageRightIm);
		m_stageHitposition = std::make_unique<Rect>(glm::vec2(middleBL, m_hitPosition), glm::vec2(m_stageHitpositionIm->getWidth(), m_stageHitpositionIm->getHeight()), 2, *m_stageHitpositionIm);
		m_stagebottom = std::make_unique<Rect>(glm::vec2(middleBL, 0), glm::vec2(m_stageBGIm->getWidth(), m_hitPosition), 4, *m_stagebottomIm);
		m_songBG = std::make_unique<Rect>(glm::vec2(0, 0), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_songBGIm);
		//Health
		m_health = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_stageRightIm->getWidth(), 0), glm::vec2(m_healthIm->getWidth(), m_healthIm->getHeight()), 2, *m_healthIm);
		m_healthBG = std::make_unique<Rect>(glm::vec2(middleBL + m_stageBGIm->getWidth() + m_stageRightIm->getWidth(), 0), glm::vec2(m_healthBGIm->getWidth(), m_healthBGIm->getHeight()), 1, *m_healthBGIm);
		//Key
		int xPos = middleBL;
		for(int i = 0; i < 7; i++)
		{
			m_key.at(i) = std::make_unique<Rect>(glm::vec2(xPos, 0), glm::vec2(m_stageBGIm->getWidth() / 7, m_keyIm.at(i)->getHeight()), 6, *m_keyIm.at(i));
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
				 glm::vec2(m_judgementIm.at(i)->getWidth(),m_judgementIm.at(i)->getHeight()), 6, *m_judgementIm.at(i));
		}

	#pragma endregion
	}

	void GameScene::Update(float dt)
	{
		if(m_t1 < 1.f)
		{
			m_leadin = lerp(2000, 0, m_t1);
			m_t1 += (1.f / 2000.f) * 1000 * dt;
		}
		else
		{
			m_leadin = 0;
			m_music->Play();
			m_music->Update();
		}
		m_curTimePos = m_music->getTimePosition();

		if(m_notesHitWeighted > 0.0f || m_notesProcessedWeighted > 0.0f)
		{
			m_accuracy = glm::clamp((m_notesHitWeighted / m_notesProcessedWeighted) * 100.f, 0.0f, 100.f);
		}

	#pragma region Conditions to results screen and animation
		if(m_totalNotesPassed >= m_totalNotes || m_gameEnded)
		{
			//Animate the screen closing and move to results screen
			m_totalTransparency -= dt * 2.0f;
			if(m_totalTransparency <= 0.0f)
			{
				g_accuracy = m_accuracy;
				g_perfCount = m_judgementCounts.at(0);
				g_greatCount = m_judgementCounts.at(1);
				g_goodCount = m_judgementCounts.at(2);
				g_badCount = m_judgementCounts.at(3);
				g_missCount = m_judgementCounts.at(4);
				g_highestCombo = m_highestCombo;
				g_CurrentScene = "result";
			}
		}

		if(m_gameEnded)
		{
			m_music->Stop();
		}

		if(m_hasHitRecently)
		{
			m_numTimer -= dt * 0.25f;
			if(m_numTimer <= 0)
			{
				m_hasHitRecently = false;
			}
		}
	#pragma endregion

	#pragma region Note spawning
		//Note spawning (Idea taken from https://www.gamedeveloper.com/programming/music-syncing-in-rhythm-games)
		//Originally for unity but the theory behind it can apply to here
		int noteX = (VPORT_WIDTH / 2) - (m_stageBGIm->getWidth() / 2);
		for (int i = 0; i < m_notes.size(); i++)
		{
			//Create a rice note if it has no release timing i.e 0 otherwise create an long note object
			if(m_nextNote.at(i) < m_notes.at(i).size() && m_notes.at(i).at(m_nextNote.at(i)).timingPoint < m_music->getTimePosition() + (2000.0f - m_cSpeed))
			{
				int column = m_notes.at(i).at(m_nextNote.at(i)).column;
				int release = m_notes.at(i).at(m_nextNote.at(i)).releasePoint;
				int timing = m_notes.at(i).at(m_nextNote.at(i)).timingPoint;
				if(release != 0)
				{
					m_noteObj.at(column).push_back(
						{std::make_shared<Rect>(glm::vec2(noteX, m_spawnPos), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, SJFOLDER + IMAGES + "notes/head" + std::to_string(i + 1) + ".png"),
						 std::make_shared<Rect>(glm::vec2(noteX, m_spawnPos), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 2, SJFOLDER + IMAGES + "notes/body" + std::to_string(i + 1) + ".png"),
						 std::make_shared<Rect>(glm::vec2(noteX, m_spawnPos), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, SJFOLDER + IMAGES + "notes/tail" + std::to_string(i + 1) + ".png")
						});
				}
				else
				{
					m_noteObj.at(column).push_back({std::make_shared<Rect>(glm::vec2(noteX, m_spawnPos), glm::vec2(m_stageBGIm->getWidth() / 7, m_noteHeight), 3, SJFOLDER + IMAGES + "notes/note" + std::to_string(i + 1) + ".png")});
				}
				m_nextNote.at(i)++;
			}
			noteX += m_stageBGIm->getWidth() / 7;
		}
	#pragma endregion

	#pragma region Note Moving
		//Note moving
		noteX = (VPORT_WIDTH / 2) - (m_stageBGIm->getWidth() / 2);	
		for(int i = 0; i < m_notes.size(); i++)
		{
			for (int j = m_notesPassed.at(i); j < m_notes.at(i).size(); j++)
			{
				//Interpolating the notes towards the hit position
				Note note = m_notes.at(i).at(j);
				int timing = note.timingPoint;
				int release = note.releasePoint;

				int lerped = lerp(m_spawnPos, m_hitPosition,
					((2000.0f - m_cSpeed) - (timing - (m_curTimePos - m_leadin))) / (2000.0f - m_cSpeed));
				int lerpedRel = lerp(m_spawnPos, m_hitPosition,
						((2000.0f - m_cSpeed) - (release - (m_curTimePos - m_leadin))) / (2000.0f - m_cSpeed));
				if(lerped <= VPORT_HEIGHT)
				{
					if(release != 0)
					{
						std::shared_ptr<Rect>& head = m_noteObj.at(i).at(j).at(0);
						std::shared_ptr<Rect>& body = m_noteObj.at(i).at(j).at(1);
						std::shared_ptr<Rect>& tail = m_noteObj.at(i).at(j).at(2);
						head->repositionVerts(glm::vec2(noteX, lerped));
						tail->repositionVerts(glm::vec2(noteX, lerpedRel));
						int length = tail->getPosition().y - head->getPosition().y;
						body->resizeVerts(glm::vec2(body->getSize().x, length));
						body->repositionVerts(head->getPosition());
					}
					else
					{
						std::shared_ptr<Rect>& rice = m_noteObj.at(i).at(j).at(0);
						rice->repositionVerts(glm::vec2(noteX, lerped));
					}
				}
				else { break; }
				//Cleanup based on miss window
				if(release != 0)
				{
					//300ms late
					if (m_curTimePos - timing >= m_missWindow && !m_holdingNote.at(i) && !m_failedRelease.at(i)) 
					{
						m_combo = 0;
						m_notesProcessedWeighted += 1.0;
						m_failedRelease.at(i) = true;
						m_hasHitRecently = true;
						m_recentJudgement = 4;
						m_hp -= m_gainLossLN;
					}
					if (m_curTimePos - release >= m_missWindow)
					{ 
						m_combo = 0;
						m_notesProcessedWeighted += 1.0; 
						m_hasHitRecently = true;
						m_failedRelease.at(i) = false;
						m_recentJudgement = 4;
						m_noteObj.at(i).at(j).clear();
						m_notesPassed.at(i)++;
						m_totalNotesPassed++;
						m_hp -= m_gainLossLN;
						m_judgementCounts.at(4)++;
					}
				}
				else if(release == 0)
				{
					//300ms late
					if (m_curTimePos - timing >= m_missWindow) 
					{ 
						m_combo = 0;
						m_notesProcessedWeighted += 1.0;
						m_hasHitRecently = true;
						m_recentJudgement = 4;
						m_noteObj.at(i).at(j).clear();
						m_notesPassed.at(i)++;
						m_totalNotesPassed++;
						m_hp -= m_gainLossLN;
						m_judgementCounts.at(4)++;
					}
				}
			}
			noteX += m_stageBGIm->getWidth() / 7;
		}
	#pragma endregion

		if (m_combo > m_highestCombo) m_highestCombo = m_combo;
	}

	void GameScene::Render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	#pragma region Stage and numbers
		//Draw stage
		m_shader->setFloat("transparency", m_totalTransparency - 0.5f);
		m_songBG->Draw(*m_shader);
		m_shader->setFloat("transparency", m_totalTransparency);
		m_stageBG->Draw(*m_shader);
		m_stageLeft->Draw(*m_shader);
		m_stageRight->Draw(*m_shader);
		m_stagebottom->Draw(*m_shader);
		m_stageHitposition->Draw(*m_shader);
		//Draw health
		if (m_hp > 100.f) m_hp = 100.f;
		if (m_hp < 0.f) m_hp = 0.f, m_gameEnded = true, g_accuracy = m_accuracy, g_failed = true;;
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
				//0 to 9 in ascii terms
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
		//Render notes as long as it is within view of the screen
		//2D array as it accesses each column and goes through each note in that column
		for (int i = 0; i < m_noteObj.size(); i++)
		{
			for (int j = m_notesPassed.at(i); j < m_noteObj.at(i).size(); j++)
			{
				for(auto& noteObj : m_noteObj.at(i).at(j))
				{
					if (noteObj->getPosition().y <= VPORT_HEIGHT && noteObj->getPosition().y + noteObj->getSize().y >= 0)
					{
						noteObj->Draw(*m_shader);
					}
				}
			}
		}

	#pragma endregion

	#pragma region Closing screen

	#pragma endregion
	}

	void GameScene::getKey(int key, int scancode, int action, int mods)
	{
		for (int i = 0; i < 7; i++)
		{
			//Check timing inputs
			if(m_notesPassed.at(i) < m_notes.at(i).size() && !m_gameEnded)
			{
				if (action == GLFW_PRESS && key == m_inputs.at(i))
				{
					//m_pressed is for showing the key pressed image
					m_pressed.at(i) = true;
					calcJudgementHit(i);
				}
				else if (action == GLFW_RELEASE && key == m_inputs.at(i))
				{
					m_pressed.at(i) = false;
					if(m_holdingNote.at(i))
					{
						calcJudgementRelease(i);
						m_holdingNote.at(i) = false;
					}
				}
			}
			else
			{
				m_pressed.at(i) = false;
			}
		}
		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) m_gameEnded = true, g_failed = true;
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

	void GameScene::calcJudgementHit(int column)
	{
		//Late: >=
		//Early: <=
		int hit = m_notes.at(column).at(m_notesPassed.at(column)).timingPoint + m_leadin;
		float difference = m_curTimePos - hit;
		//Increase combo or reset combo depending if its a perfect to good or bad to a miss
		//Display the combo and judge
		//Calculate accuracy
		//Remove the note if its at the end of the long note
		if(difference <= m_perfWindow && difference >= -m_perfWindow)
		{
			hitJudgement(column, 0, m_perfWeight);
		}
		else if (difference <= m_greatWindow && difference >= -m_greatWindow)
		{
			hitJudgement(column, 1, m_greatWeight);
		}
		else if (difference <= m_goodWindow && difference >= -m_goodWindow)
		{
			hitJudgement(column, 2, m_goodWeight);
		}
		else if (difference <= m_badWindow && difference >= -m_badWindow)
		{
			hitJudgement(column, 3, m_badWeight);
		}
		else if (difference <= m_missWindow && difference >= -m_missWindow)
		{
			hitJudgement(column, 4, m_missWeight);
		}
	}

	void GameScene::hitJudgement(int column,int recent, float weight)
	{
		if(!m_failedRelease.at(column) || m_noteObj.at(column).at(m_notesPassed.at(column)).size() == 1)
		{
			if(recent != 4) { m_combo++; }
			else { m_combo = 0; }
			if (recent == 3) m_hp -= weight * m_gainLossRice * 2.0f;
			else if (recent == 4) m_hp -= m_gainLossRice * 2.0f;
			else m_hp += weight * m_gainLossRice;
			m_hasHitRecently = true;
			m_recentJudgement = recent;
			m_notesHitWeighted += weight;
			m_notesProcessedWeighted += 1.0;
			m_judgementCounts.at(recent)++;
			m_numTimer = 5.0f;
		}

		if (m_noteObj.at(column).at(m_notesPassed.at(column)).size() == 1)
		{	
			m_noteObj.at(column).at(m_notesPassed.at(column)).clear();
			m_notesPassed.at(column)++;
			m_totalNotesPassed++;
		}
		else
		{
			m_holdingNote.at(column) = true;
		}
	}

	void GameScene::calcJudgementRelease(int column)
	{
		//Giving the release window a slightly wider window since I think releases are harder than hitting
		int release = m_notes.at(column).at(m_notesPassed.at(column)).releasePoint + m_leadin;
		float difference = m_curTimePos - release;
		if (difference <= m_perfWindow * m_windowMult && difference >= -m_perfWindow * m_windowMult)
		{
			releaseJudgement(column, 0, m_perfWeight);
		}
		else if (difference <= m_greatWindow * m_windowMult && difference >= -m_greatWindow * m_windowMult)
		{
			releaseJudgement(column, 1, m_greatWeight);
		}
		else if (difference <= m_goodWindow * m_windowMult && difference >= -m_goodWindow * m_windowMult)
		{
			releaseJudgement(column, 2, m_goodWeight);
		}
		else if (difference <= m_badWindow * m_windowMult && difference >= -m_badWindow * m_windowMult)
		{
			releaseJudgement(column, 3, m_badWeight);
		}
		else
		{
			m_combo = 0;
			m_hasHitRecently = true;
			m_failedRelease.at(column) = true;
			m_recentJudgement = 4;
		}
	}

	void GameScene::releaseJudgement(int column, int recent, float weight)
	{
		m_combo++;
		m_hasHitRecently = true;
		m_recentJudgement = recent;
		m_notesHitWeighted += weight;
		m_notesProcessedWeighted += 1.0;
		m_numTimer = 5.0f;
		if (m_noteObj.at(column).at(m_notesPassed.at(column)).size() == 3)
		{
			if (recent == 3) m_hp -= weight * m_gainLossLN * 2.0f;
			else m_hp += weight * m_gainLossLN;
			m_noteObj.at(column).at(m_notesPassed.at(column)).clear();
			m_notesPassed.at(column)++;
			m_judgementCounts.at(recent)++;
			m_totalNotesPassed++;
		}
	}
}
/*****************************************************************//**
 * \file   ResultsScene.cpp
 * \brief  the results screen
 *
 * \date   April 2023
 *********************************************************************/
#include "Scenes/ResultsScene.h"
#include "Utils/Properties.h"

namespace SJ
{
	ResultsScene::ResultsScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		//Reenable cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "click.mp3");

		//Create shaders
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_shader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_shader->setMat4("projection", projection);

		m_textShader = std::make_unique<Shader>(SJFOLDER + SHADER + "text.vert", SJFOLDER + SHADER + "text.frag");
		m_textShader->setMat4("projection", projection);

	#pragma region texture creation
		//Create textures
		m_songIm = std::make_unique<Texture>(m_folder + g_CurrentOsuDir + L"/" + g_CurrentBG, GL_CLAMP_TO_EDGE);
		m_gradesBGIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "resultsbg.png", GL_CLAMP_TO_EDGE);

		std::array<std::string, 8> gradePaths = { "GradeP.png", "GradeAAA.png" ,"GradeAA.png" ,"GradeA.png"
			,"GradeB.png" ,"GradeC.png" ,"GradeD.png" ,"GradeF.png" };
		for (int i = 0; i < 8; i++)
		{
			m_gradesIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + gradePaths.at(i), GL_CLAMP_TO_EDGE);
		}

		std::array<std::string, 5> judgeFiles =
		{ "perfectsmall.png","greatsmall.png","goodsmall.png","badsmall.png","misssmall.png" };
		for (int i = 0; i < 5; i++)
		{
			m_judgementIm.at(i) = std::make_unique<Texture>(SJFOLDER + IMAGES + "game/" + judgeFiles.at(i), GL_CLAMP_TO_EDGE);
		}
	#pragma endregion

	#pragma region object creation
		//Create objects
		m_song = std::make_unique<Rect>(glm::vec2(0.f,0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_songIm);
		m_gradesBG = std::make_unique<Rect>(glm::vec2(200.0f, 0.f), glm::vec2(880.f, 475.2f), 1, *m_gradesBGIm);

		for(int i = 0; i < 8; i++)
		{
			m_grades.at(i) = std::make_unique<Rect>(glm::vec2(680.0f, 200.f), glm::vec2(m_gradesIm.at(i)->getWidthf(), m_gradesIm.at(0)->getHeightf()), 2 , *m_gradesIm.at(i));
		}

		m_judgement.at(0) = std::make_unique<Rect>(glm::vec2(200.0f, 405.f), glm::vec2(m_judgementIm.at(0)->getWidthf(), m_judgementIm.at(0)->getHeightf()), 2, *m_judgementIm.at(0));
		m_judgement.at(1) = std::make_unique<Rect>(glm::vec2(200.0f, 305.f), glm::vec2(m_judgementIm.at(1)->getWidthf(), m_judgementIm.at(1)->getHeightf()), 2, *m_judgementIm.at(1));
		m_judgement.at(2) = std::make_unique<Rect>(glm::vec2(200.0f, 205.f), glm::vec2(m_judgementIm.at(2)->getWidthf(), m_judgementIm.at(2)->getHeightf()), 2, *m_judgementIm.at(2));
		m_judgement.at(3) = std::make_unique<Rect>(glm::vec2(200.0f, 105.f), glm::vec2(m_judgementIm.at(3)->getWidthf(), m_judgementIm.at(3)->getHeightf()), 2, *m_judgementIm.at(3));
		m_judgement.at(4) = std::make_unique<Rect>(glm::vec2(200.0f, 5.f) , glm::vec2(m_judgementIm.at(4)->getWidthf(), m_judgementIm.at(4)->getHeightf()), 2, *m_judgementIm.at(4));

		m_perfCount = std::make_unique<Text>(glm::vec2(210.f + m_judgementIm.at(0)->getWidthf(), 395.f), L": " + std::to_wstring(g_perfCount), 200, 48, 2, "NotoSansJP-Light.otf");
		m_greatCount = std::make_unique<Text>(glm::vec2(210.f + m_judgementIm.at(1)->getWidthf(), 295.f), L": " + std::to_wstring(g_greatCount), 200, 48, 2, "NotoSansJP-Light.otf");
		m_goodCount = std::make_unique<Text>(glm::vec2(210.f + m_judgementIm.at(2)->getWidthf(), 195.f), L": " + std::to_wstring(g_goodCount), 200, 48, 2, "NotoSansJP-Light.otf");
		m_badCount = std::make_unique<Text>(glm::vec2(210.f + m_judgementIm.at(3)->getWidthf(), 95.f), L": " + std::to_wstring(g_badCount), 200, 48, 2, "NotoSansJP-Light.otf");
		m_missCount = std::make_unique<Text>(glm::vec2(210.f + m_judgementIm.at(4)->getWidthf(), -5.f), L": " + std::to_wstring(g_missCount), 200, 48, 2, "NotoSansJP-Light.otf");

		m_title = std::make_unique<Text>(glm::vec2(0, 0), g_CurrentTitle, g_CurrentTitle.size() * 86, 86, 3, "NotoSansJP-Bold.otf");
		m_title->repositionVerts(glm::vec2((VPORT_WIDTH / 2) - (m_title->getBitmapWidth() / 2), VPORT_HEIGHT - 128));

		m_difficulty = std::make_unique<Text>(glm::vec2(0, 0), g_CurrentDiffName, g_CurrentDiffName.size() * 48, 48, 3, "NotoSansJP-Medium.otf");
		m_difficulty->repositionVerts(glm::vec2((VPORT_WIDTH / 2) - (m_difficulty->getBitmapWidth() / 2), VPORT_HEIGHT - 200));

		m_highestCombo = std::make_unique<Text>(glm::vec2(480.f, 10.f), std::to_wstring(g_highestCombo) + L"x", 500, 64, 3, "NotoSansJP-Regular.otf");
		m_highestCombo->repositionVerts(glm::vec2((1100.f / 2) - (m_highestCombo->getBitmapWidth() / 2), 10.f));

		m_percent = std::make_unique<Text>(glm::vec2(720.f, 0.f), 
			std::to_wstring(g_accuracy).substr(0, std::to_wstring(g_accuracy).find_first_of('.') + 3) + L"%", 500, 96, 3, "NotoSansJP-Regular.otf");
	#pragma endregion
	}

	ResultsScene::~ResultsScene()
	{
		m_sfx->removeSFX(m_anyKeySound);
	}

	void ResultsScene::Update(float dt)
	{
		if(m_goBackToSelect)
		{
			m_totalTransparency -= dt * 2.0f;
		}
		if(m_totalTransparency <= 0.0f)
		{
			g_CurrentScene = "song_select";
		}
	}

	void ResultsScene::Render()
	{
		m_shader->setFloat("transparency", m_totalTransparency - 0.5f);
		m_song->Draw(*m_shader);
		m_shader->setFloat("transparency", m_totalTransparency - 0.3f);
		m_gradesBG->Draw(*m_shader);

		m_shader->setFloat("transparency", m_totalTransparency);
		for (int i = 0; i < 5; i++) { m_judgement.at(i)->Draw(*m_shader); }

		m_textShader->setFloat("transparency", m_totalTransparency);
		m_perfCount->Draw(*m_textShader);
		m_greatCount->Draw(*m_textShader);
		m_goodCount->Draw(*m_textShader);
		m_badCount->Draw(*m_textShader);
		m_missCount->Draw(*m_textShader);
		m_title->Draw(*m_textShader);
		m_difficulty->Draw(*m_textShader);
		m_highestCombo->Draw(*m_textShader);
		m_percent->Draw(*m_textShader);

		if (g_failed) { m_grades.at(7)->Draw(*m_shader);}
		else
		{
			for(int i = 0; i < 7; i++)
			{
				if(g_accuracy > m_gradeThresholds.at(i))
				{
					m_grades.at(i)->Draw(*m_shader);
					break;
				}
			}
		}
	}

	void ResultsScene::getKey(int key, int scancode, int action, int mods)
	{
		if(action == GLFW_PRESS && key == GLFW_KEY_ENTER && !m_goBackToSelect)
		{
			{
				m_source = std::make_unique<SFXSource>();
				m_source->Play(m_anyKeySound);
			}
			m_goBackToSelect = true;
		}
	}

	void ResultsScene::getMouseButton(int button, int action, int mods)
	{
		if(action == GLFW_PRESS && !m_goBackToSelect)
		{
			{
				m_source = std::make_unique<SFXSource>();
				m_source->Play(m_anyKeySound);
			}
			m_goBackToSelect = true;
		}
	}

	void ResultsScene::getScroll(double xoffset, double yoffset)
	{
	}

	void ResultsScene::fileDrop(int count, const char** paths)
	{
	}
}
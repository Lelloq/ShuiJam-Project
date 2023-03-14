/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"
#include "Utils/Properties.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <future>

namespace SJ
{
	MenuScene::MenuScene(GLFWwindow* window) : m_window(window), m_device(AudioDevice::get()), m_sfx(SoundEffect::get())
	{
		m_source = std::make_unique<SJ::SFXSource>();
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "start.wav");

		m_bgIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "titlebg.png", GL_CLAMP_TO_EDGE);
		m_bg = std::make_unique<Rect>(glm::vec2(0.f, 0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_bgIm);

		m_titleIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_title = std::make_unique<Rect>(glm::vec2(465.f, 250.f), glm::vec2(350.f, 350.f), 1, *m_titleIm);

		m_startIm = std::make_unique<Texture>(SJFOLDER + IMAGES + "starttext.png", GL_CLAMP_TO_EDGE);
		m_start = std::make_unique<Rect>(glm::vec2(520.f, 100.f), glm::vec2(240.f, 25.f), 1, *m_startIm);

		glm::mat4 model{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1.f) };
		m_bgShader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_titleShader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");
		m_startShader = std::make_unique<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_bgShader->use();
		m_bgShader->setMat4("model", model);
		m_bgShader->setMat4("view", view);
		m_bgShader->setMat4("projection", projection);

		m_titleShader->use();
		m_titleShader->setMat4("model", model);
		m_titleShader->setMat4("view", view);
		m_titleShader->setMat4("projection", projection);

		m_startShader->use();
		m_startShader->setMat4("model", model);
		m_startShader->setMat4("view", view);
		m_startShader->setMat4("projection", projection);
	}

	//Animate objects
	void MenuScene::Update(float dt)
	{
		m_titleShader->use();
		m_titleShader->setFloat("transparency", m_intermediate);
		if(m_intermediate < 1.f)
		{
			m_intermediate += dt;
		}

		m_startShader->use();
		m_startShader->setFloat("transparency", m_toggle);
		m_toggleValue += dt;
		if(m_toggleValue > m_toggleThreshold)
		{
			m_toggle = (m_toggle == true) ? false : true;
			m_toggleValue = 0;
		}

		if(m_pressed)
		{
			m_timer += dt;
			if(m_timer > 1)
			{
				m_sceneTransparency -= dt * 7;
				m_bgShader->use();
				m_bgShader->setFloat("transparency", m_sceneTransparency);
				m_titleShader->use();
				m_bgShader->setFloat("transparency", m_sceneTransparency);
				m_startShader->use();
				m_bgShader->setFloat("transparency", m_sceneTransparency);
			}
			if(m_sceneTransparency <= -3)
			{
				g_CurrentScene = "song_select";
				m_sceneTransparency = 0;
			}
		}
	}

	//Draw static objects
	void MenuScene::Render()
	{
		glClearColor(0, 0, 0, 0);
		m_bg->Draw(*m_bgShader);
		m_title->Draw(*m_titleShader);
		m_start->Draw(*m_startShader);
	}

	void MenuScene::getKey(int key, int scancode, int action, int mods)
	{
		if(action == GLFW_PRESS && !m_pressed)
		{
			m_source->Play(m_anyKeySound);
			m_toggleThreshold = 0.05f;
			m_pressed = true;
		}
	}
	void MenuScene::getMouseButton(int button, int action, int mods)
	{
		/*
		if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);
			x = x * (VPORT_WIDTH / SCR_WIDTH);
			y = VPORT_HEIGHT - (y * (VPORT_HEIGHT / SCR_HEIGHT));
			std::cout << "x: " << x << "y: " << y << "\n";
		}
		*/
	}
	void MenuScene::getScroll(double xoffset, double yoffset)
	{
	}
	void MenuScene::fileDrop(int count, const char** paths)
	{
	}
}

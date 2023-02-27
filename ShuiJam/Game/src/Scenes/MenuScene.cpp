/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"
#include "Utils/Properties.h"
#include <GLFW/glfw3.h>

namespace SJ
{
	MenuScene::MenuScene(GLFWwindow* window) : m_window(window), m_device(SJ::AudioDevice::get()), m_sfx(SJ::SoundEffect::get())
	{
		m_source = std::make_shared<SJ::SFXSource>();
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "start.wav");

		m_bg = std::make_shared<Texture>(SJFOLDER + IMAGES + "titlebg.png", GL_CLAMP_TO_EDGE);
		m_titleBG = std::make_shared<Rect>(glm::vec2(0.f, 0.f), glm::vec2(VPORT_WIDTH, VPORT_HEIGHT), 0, *m_bg);

		m_title = std::make_shared<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_titleText = std::make_shared<Rect>(glm::vec2(0.f, 0.f), glm::vec2(200.f, 200.f), 1, *m_title);

		m_text = std::make_shared<Texture>(SJFOLDER + IMAGES + "starttext.png", GL_CLAMP_TO_EDGE);
		m_startText = std::make_shared<Rect>(glm::vec2(0.f, 0.f), glm::vec2(200.f, 100.f), 2, *m_text);

		glm::mat4 model{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, VPORT_WIDTH, 0.f, VPORT_HEIGHT, -1000.f, 1000.f) };
		m_shader = std::make_shared<Shader>(SJFOLDER + SHADER + "title.vert", SJFOLDER + SHADER + "title.frag");

		m_shader->use();
		m_shader->setInt("image", 0);
		m_shader->setFloat("transparency", 1.0f);
		m_shader->setMat4("model", model);
		m_shader->setMat4("view", view);
		m_shader->setMat4("projection", projection);
	}

	void MenuScene::Update(float dt)
	{
	}

	void MenuScene::Render()
	{
		Renderer::Draw(m_titleBG->getVAO(), m_titleBG->getEBO(), *m_shader.get());
	}

	void MenuScene::getKey(int key, int scancode, int action, int mods)
	{
		if(action == GLFW_PRESS && !m_pressed)
		{
			m_source->Play(m_anyKeySound);
			m_pressed = true;
			g_CurrentScene = "song_select";
		}
	}
	void MenuScene::getMouseButton(int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);
			x = x * (VPORT_WIDTH / 1366.f);
			y = y * (VPORT_HEIGHT / 768.f);
			std::cout << "x: " << x << "y: " << y << "\n";
		}
	}
	void MenuScene::getScroll(double xoffset, double yoffset)
	{
	}
	void MenuScene::fileDrop(int count, const char** paths)
	{
	}
}

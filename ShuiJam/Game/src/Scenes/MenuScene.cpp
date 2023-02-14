/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"
#include "Utils/Properties.h"

namespace SJ
{
	MenuScene::MenuScene(GLFWwindow* window) : m_window(window), m_device(SJ::AudioDevice::get()), m_sfx(SJ::SoundEffect::get())
	{
		m_source = std::make_shared<SJ::SFXSource>();
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "hitclap.wav");

		m_image = std::make_shared<Texture>(SJFOLDER + IMAGES + "title.png", GL_CLAMP_TO_EDGE);
		m_titleBG = std::make_shared<Rect>(glm::vec2(0.f, 0.f), glm::vec2(WIDTH, HEIGHT), 0, *m_image);

		glm::mat4 model{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, static_cast<float>(WIDTH), 0.f, static_cast<float>(HEIGHT), -1000.f, 1000.f) };
		m_shader = std::make_shared<Shader>(SJFOLDER + SHADER + "basic.vert", SJFOLDER + SHADER + "basic.frag");

		m_shader->use();
		m_shader->setInt("image", 0);
		m_shader->setMat4("model", model);
		m_shader->setMat4("view", view);
		m_shader->setMat4("projection", projection);
	}

	void MenuScene::Update(float dt)
	{
	}

	void MenuScene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Renderer::Draw(m_titleBG->getVAO(), m_titleBG->getEBO(), *m_shader);
	}

	void MenuScene::getKey(int key, int scancode, int action, int mods)
	{
		if(action == GLFW_PRESS)
		{
			m_source->Play(m_anyKeySound);
			std::cout << key << std::endl;
		}
	}
	void MenuScene::getMouseButton(int button, int action, int mods)
	{
	}
	void MenuScene::getScroll(double xoffset, double yoffset)
	{
	}
	void MenuScene::fileDrop(int count, const char** paths)
	{
	}
}

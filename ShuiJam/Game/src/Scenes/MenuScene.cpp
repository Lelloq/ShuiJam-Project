/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"
#include "Utils/Properties.h"
#include <GLFW/glfw3.h>

namespace SJ
{
	MenuScene::MenuScene() : m_sfx(SJ::SoundEffect::get())
	{
		m_source = std::make_shared<SJ::SFXSource>();
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "start.wav");

		m_bg = std::make_shared<Texture>(SJFOLDER + IMAGES + "titlebg.png", GL_CLAMP_TO_EDGE);
		m_titleBG = std::make_shared<Rect>(glm::vec2(0.f, 0.f), glm::vec2(SCR_WIDTH, SCR_HEIGHT), 0, *m_bg);

		glm::mat4 model{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ glm::ortho(0.f, static_cast<float>(SCR_WIDTH), 0.f, static_cast<float>(SCR_HEIGHT), -1000.f, 1000.f) };
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
	}
	void MenuScene::getScroll(double xoffset, double yoffset)
	{
	}
	void MenuScene::fileDrop(int count, const char** paths)
	{
	}
}

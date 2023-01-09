/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"
#include "Utils/Properties.h"

namespace SJ
{
	MenuScene::MenuScene(GLFWwindow* window)
	{
		m_window = window;
		m_device = SJ::AudioDevice::get();
		m_sfx = SJ::SoundEffect::get();

		m_source.reset(new SJ::SFXSource);
		m_anyKeySound = m_sfx->addSFX(SJFOLDER + SOUNDS + "hitclap.wav");
	}

	MenuScene::~MenuScene()
	{
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

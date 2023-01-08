/*\file MenuScenec.pp*/
#include "Scenes/MenuScene.h"

namespace SJ
{
	MenuScene::MenuScene(GLFWwindow* window)
	{
		m_window = window;
		m_device = SJ::AudioDevice::get();
		m_sfx = SJ::SoundEffect::get();

		m_source.reset(new SJ::SFXSource);
		m_anyKeySound = m_sfx->addSFX("../ShuiJamGame/Sounds/hitclap.wav");
	}

	MenuScene::~MenuScene()
	{
	}

	void MenuScene::getKey(int key, int action)
	{
		if(action == GLFW_PRESS)
		{
			m_source->Play(m_anyKeySound);
			std::cout << key << std::endl;
		}
	}
}

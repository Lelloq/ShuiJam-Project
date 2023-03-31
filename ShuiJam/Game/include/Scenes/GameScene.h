/*****************************************************************//**
 * \file   GameScene.h
 * \brief  main gameplay
 * 
 * \date   March 2023
 *********************************************************************/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "objects/objects.h"
#include "Utils/OsuParser.h"
#include <GLFW/glfw3.h>

namespace SJ
{
	class GameScene : public Scene
	{
	private:
		//WINDOW
		GLFWwindow* m_window;
		//AUDIO
		SJ::AudioDevice* m_device;//!< Audio device
		SJ::SoundEffect* m_sfx;//!< Sound effect
	public:
		GameScene(GLFWwindow* window);
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}

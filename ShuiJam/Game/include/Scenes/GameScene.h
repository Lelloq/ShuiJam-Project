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
		AudioDevice* m_device;//!< Audio device
		SoundEffect* m_sfx;//!< Sound effect
		std::unique_ptr<Music> m_music;
		//GRAPHICS
		//Shader
		std::shared_ptr<Shader> m_shader;
		//Note images
		std::array<std::shared_ptr<Texture>, 7> m_riceIm;
		std::array<std::shared_ptr<Texture>, 7> m_headIm;
		std::array<std::shared_ptr<Texture>, 7> m_tailIm;
		std::vector<std::unique_ptr<Rect>> m_rice;
		std::vector<std::unique_ptr<Rect>> m_head;
		std::vector<std::unique_ptr<Rect>> m_tail;
		//Stage
		std::unique_ptr<Texture> m_stageLeftIm;
		std::unique_ptr<Texture> m_stageRightIm;
		std::unique_ptr<Texture> m_stageHitpositionIm;
		std::unique_ptr<Texture> m_stageBGIm;
		std::unique_ptr<Texture> m_healthBGIm;
		std::unique_ptr<Texture> m_healthIm;
		std::array<std::unique_ptr<Texture>, 7> m_keyIm;
		std::unique_ptr<Rect> m_stageLeft;
		std::unique_ptr<Rect> m_stageRight;
		std::unique_ptr<Rect> m_stageHitposition;
		std::unique_ptr<Rect> m_stageBG;
		std::unique_ptr<Rect> m_healthBG;
		std::unique_ptr<Rect> m_health;
		std::array<std::unique_ptr<Rect>, 7> m_key;
		//Combo
		std::array<std::shared_ptr<Texture>, 10> m_comboNumIm;
		std::array<std::shared_ptr<Rect>, 10> m_comboNum;
		//Percent
		std::shared_ptr<Texture> m_dotIm;
		std::shared_ptr<Texture> m_percentIm;
		std::shared_ptr<Rect> m_percent;
		std::shared_ptr<Rect> m_dot;
		//Judgement
		std::array<std::unique_ptr<Texture>, 5> m_judgementIm;
		std::array<std::unique_ptr<Rect>, 5> m_judgement;
		//OTHER DATA
		std::array<std::vector<Note>, 7> m_notes;//Note data for the song
		std::wstring m_folder = L"../ShuiJamGame/Songs/";
		std::array<int, 7> m_inputs = //Default keyboard inputs
		{GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F , GLFW_KEY_SPACE, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L};
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

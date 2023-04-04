/*****************************************************************//**
 * \file   SongScene.h
 * \brief  Song select scene where the player selects a song to play in the game
 * 
 * \date   February 2023
 *********************************************************************/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "objects/objects.h"
#include "Utils/FileProcessor.h"
#include <GLFW/glfw3.h>

namespace SJ
{
	/**
	 * \class SongScene
	 * \brief The song select scene.
	 */
	class SongScene : public Scene
	{
	private:
		//Database
		std::unique_ptr<FileProcessor> m_fileProcessor;
		//WINDOW
		GLFWwindow* m_window;
		//AUDIO
		SJ::AudioDevice* m_device;
		SJ::SoundEffect* m_sfx;
		std::unique_ptr<SFXSource> m_source;
		std::unique_ptr<Music> m_music;
		ALuint m_scrollSound, m_startSound, m_refreshSound;
		//GRAPHICS
		//Buttons
		std::vector<std::unique_ptr<Button>> m_buttons;//Buttons in song select 11 buttons
		std::vector<std::unique_ptr<Button>> m_sbuttons;//Buttons in settings 2 buttons
		//Shader
		std::unique_ptr<Shader> m_shader;
		//Textures
		std::unique_ptr<Texture> m_songBGIm;
		std::unique_ptr<Texture> m_songSelectIm;
		std::shared_ptr<Texture> m_selectWheelIm;
		std::unique_ptr<Texture> m_logoIm;
		//Buttons
		std::unique_ptr<Button> m_logoBtn;
		//Objects
		std::unique_ptr<Rect> m_songBG;
		std::unique_ptr<Rect> m_songSelect;
		//Text
		std::unique_ptr<Shader> m_textShader;
		std::vector<std::unique_ptr<Text>> m_songWheelText;
		//Values
		std::vector<int> m_buttonPositions;//positions of the song wheel
		int m_upperLimit = 686;
		int m_lowerLimit = 3;
		int m_scrollDirection = 0;
		bool m_scrollDebounce = false;
		float m_slow = 0;
		int m_pixels = 0;
		//Song data storage for the scene
		std::array<Songdata, 12> m_songData;
		unsigned m_head = 0;
		unsigned m_tail = 11;
		void updateSongWheel();

		//GRAPHICS FOR SETTINGS SCREEN
		std::unique_ptr<Texture> m_settingsIm;
		std::unique_ptr<Texture> m_keybindIm;
		std::unique_ptr<Button> m_keybindBtn;
		std::unique_ptr<Button> m_sButtonmBtn;
	public:
		SongScene(GLFWwindow* window);
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}

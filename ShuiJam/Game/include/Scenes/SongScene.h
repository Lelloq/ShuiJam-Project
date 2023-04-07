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
#include <map>

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
		AudioDevice* m_device;
		SoundEffect* m_sfx;
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
		std::unique_ptr<Texture> m_songBGIm, m_songSelectIm, m_logoIm;
		std::shared_ptr<Texture> m_selectWheelIm;
		//Buttons
		std::unique_ptr<Button> m_logoBtn;
		//Objects
		std::unique_ptr<Rect> m_songBg;
		std::unique_ptr<Rect> m_songSelect;
		//Text
		std::unique_ptr<Shader> m_textShader;
		std::vector<std::unique_ptr<Text>> m_songWheelText;
		std::unique_ptr<Text> m_songText, m_artistText, m_diffText;
		//Values
		std::vector<int> m_buttonPositions;//positions of the song wheel
		bool m_canClick = true;
		int m_upperLimit = 687;//Highest point for the image before wrapping around
		int m_lowerLimit = 60;
		int m_scrollDirection = 0;
		bool m_scrollDebounce = false;//Prevents extremely fast scrolling
		int m_confirmation;//Highlighting the selected song
		float m_slow = 0;
		int m_pixels = 0;
		double m_cursorPosX, m_cursorPosY;
		//Song data storage for the scene
		std::array<Songdata, 11> m_songData;
		int m_top = 0;
		void updateSongWheel();

		//GRAPHICS FOR EXITING THE GAME
		bool m_exitOpen = false;//Whether or no the logo button has been pressed (shows the exit button)
		std::unique_ptr<Texture> m_exitBgIm;
		std::unique_ptr<Rect> m_exitBg;
		std::unique_ptr<Texture> m_exitYesIm, m_exitNoIm;
		std::unique_ptr<Button> m_exitYesBtn, m_exitNoBtn;

		//GRAPHICS FOR SETTINGS SCREEN
		bool m_settingsOpen = false;//Settings ui open or not
		bool m_keybindsOpen = false;//Keybinds ui open or not
		std::unique_ptr<Texture> m_settingsIm, m_keybindIm;
		std::unique_ptr<Button> m_keybindBtn, m_sButtonmBtn;
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

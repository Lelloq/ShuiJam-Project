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
#include <array>

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
		bool m_isPlaying = false;
		//GRAPHICS
		//Buttons
		std::vector<std::unique_ptr<Button>> m_buttons;//Buttons in song select 11 buttons
		//Shader
		std::unique_ptr<Shader> m_shader;
		//Textures
		std::unique_ptr<Texture> m_songBGIm, m_songSelectIm, m_logoIm;
		std::unique_ptr<Texture> m_songBGImPlay;//This appears on top of everything when the player presses play
		std::shared_ptr<Texture> m_selectWheelIm;
		//Buttons
		std::unique_ptr<Button> m_logoBtn;
		//Objects
		std::unique_ptr<Rect> m_songBg;
		std::unique_ptr<Rect> m_songBGPlay;//This appears on top of everything when the player presses play
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
		int m_confirmation = -1;//Highlighting the selected song
		float m_slow = 0;
		int m_pixels = 0;
		double m_cursorPosX, m_cursorPosY;
		bool m_canScrollDown = true;
		bool m_canScrollUp = false;
		//Song data storage for the scene
		std::array<Songdata, 11> m_songData;
		int m_head = 0;
		int m_tail = 11;
		int m_lastSong = 0;

		/**
		 * \function void updateSongWheel().
		 * \brief Updates the song wheel text based on the song title data within the songs database from head to tail
		 */
		void updateSongWheel();

		/**
		 * \function void startGame().
		 * \brief Swaps to the GameScene and passes on the selected song data through global variables
		 */
		void startGame();

		/**
		 * \function void scrollDown().
		 * \brief Scrolls down the song wheel and increments the head and tail as well as stops the current selected audio
		 */
		void scrollDown();

		/**
		 * \function void scrollUp().
		 * \brief Scrolls down the song wheel and increments the head and tail as well as stops the current selected audio
		 */
		void scrollUp();

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
		std::unique_ptr<Button> m_keybindBtn, m_settingsBtn;
		std::array<std::unique_ptr<Button>, 7> m_keysBtn;//Key button
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

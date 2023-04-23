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
#include "Utils/Settings.h"
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
		std::unique_ptr<Texture> m_settingsIm;
		std::unique_ptr<Button> m_settingsBtn;

		//Images
		std::unique_ptr<Texture> m_settingsBGIm;
		std::shared_ptr<Texture> m_leftButtonIm;
		std::shared_ptr<Texture> m_rightButtonIm;
		std::unique_ptr<Texture> m_resolutionButtonIm;
		std::shared_ptr<Texture> m_blankButtonIm;
		std::shared_ptr<Texture> m_checkButtonIm;
		//Objects
		std::unique_ptr<Rect> m_settingsBG;

		std::unique_ptr<Button> m_offsetButtonL;
		std::unique_ptr<Button> m_offsetButtonR;
		std::unique_ptr<Text> m_offsetText;
		std::unique_ptr<Text> m_offsetNum;

		std::unique_ptr<Button> m_hitposButtonL;
		std::unique_ptr<Button> m_hitposButtonR;
		std::unique_ptr<Text> m_hitposText;
		std::unique_ptr<Text> m_hitposNum;

		std::unique_ptr<Button> m_volumeButtonL;
		std::unique_ptr<Button> m_volumeButtonR;
		std::unique_ptr<Text> m_volumeText;
		std::unique_ptr<Text> m_volumeNum;
		//Values
		bool m_shiftHeld = false;

		//Objects for keybinds
		std::unique_ptr<Texture> m_keybindIm;
		std::unique_ptr<Button> m_keybindBtn;

		std::unique_ptr<Texture> m_keybindBGIm;
		std::unique_ptr<Rect> m_keybindBG;
		std::array<std::unique_ptr<Texture>, 7> m_keyIm;
		std::array<std::unique_ptr<Rect>, 7> m_key;//Key button

		std::shared_ptr<Texture> m_keyBtnIm;
		std::array<std::unique_ptr<Button>, 7> m_keyBtn;//Key button
		std::array<std::unique_ptr<Text>, 7> m_keyTxt;

		//Values
		std::array<int, 7> m_inputs = //keyboard inputs for the tester
		{ g_keyOne, g_keyTwo, g_keyThree , g_keyFour, g_keyFive, g_keySix, g_keySeven };
		std::array<bool, 7> m_pressed = { false, false, false, false, false ,false ,false };
		std::array<bool, 7> m_keyForChange = { false, false, false, false, false ,false ,false };

		bool m_changing = false;

		//Audio functions
		void playRefresh();
		void playStart();
		void playScroll();
	public:
		SongScene(GLFWwindow* window);
		/**
		 * \function void Update(float dt).
		 * \brief Update visuals such as positions and other data
		 *
		 * \param dt deltatime
		 */
		void Update(float dt);

		/**
		 * \function void Render().
		 * \brief Renders the graphics on screen using OpenGL
		 */
		void Render();

		/**
		 * \function getKey(int key, int scancode, int action, int mods) override.
		 * \brief Callbacks that gets keyboard inputs using glfw
		 *
		 * \param key
		 * \param scancode
		 * \param action
		 * \param mods
		 */
		void getKey(int key, int scancode, int action, int mods) override;

		/**
		 * \function void getMouseButton(int button, int action, int mods) override.
		 * \brief Callbacks that gets mouse button inputs using glfw
		 *
		 * \param button
		 * \param action
		 * \param mods
		 */
		void getMouseButton(int button, int action, int mods) override;

		/**
		 * \function void getScroll(double xoffset, double yoffset) override.
		 * \brief Callbacks that gets the scroll direction of the scroll wheel using glfw
		 *
		 * \param xoffset
		 * \param yoffset
		 */
		void getScroll(double xoffset, double yoffset) override;

		/**
		 * \function void fileDrop(int count, const char** paths) override.
		 *
		 * \param count how many files there are
		 * \param paths file path
		 */
		void fileDrop(int count, const char** paths) override;
	};
}

/*****************************************************************//**
 * \file   GameScene.h
 * \brief  main gameplay scene of the rhythm game
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
#include "Utils/Settings.h"

namespace SJ
{
	/**
	 * \class GameScene
	 * \brief the GameScene is the main scene where all the game logic occurs such as note spawning and syncing.
	 */
	class GameScene : public Scene
	{
	private:
		//WINDOW
		GLFWwindow* m_window;
		//AUDIO
		AudioDevice* m_device;//!< Audio device
		SoundEffect* m_sfx;//!< Sound effect
		std::unique_ptr<Music> m_music;
		bool m_playing = false;
		//GRAPHICS
		//Shader
		std::shared_ptr<Shader> m_shader;

		//Stage
		std::unique_ptr<Texture> m_songBGIm;
		std::unique_ptr<Texture> m_stageLeftIm;
		std::unique_ptr<Texture> m_stageRightIm;
		std::unique_ptr<Texture> m_stageHitpositionIm;
		std::unique_ptr<Texture> m_stagebottomIm;
		std::unique_ptr<Texture> m_stageBGIm;
		std::unique_ptr<Texture> m_healthBGIm;
		std::unique_ptr<Texture> m_healthIm;
		std::array<std::unique_ptr<Texture>, 7> m_keyIm;
		std::unique_ptr<Rect> m_songBG;
		std::unique_ptr<Rect> m_stageLeft;
		std::unique_ptr<Rect> m_stageRight;
		std::unique_ptr<Rect> m_stageHitposition;
		std::unique_ptr<Rect> m_stageBG;
		std::unique_ptr<Rect> m_stagebottom;
		std::unique_ptr<Rect> m_healthBG;
		std::unique_ptr<Rect> m_health;
		std::array<std::unique_ptr<Rect>, 7> m_key;

		//Combo
		std::array<std::shared_ptr<Texture>, 10> m_numIm;
		std::array<std::shared_ptr<Rect>, 10> m_num;

		//Percent
		std::shared_ptr<Texture> m_dotIm;
		std::shared_ptr<Texture> m_percentIm;
		std::shared_ptr<Rect> m_percent;
		std::shared_ptr<Rect> m_dot;

		//Judgement
		std::array<std::unique_ptr<Texture>, 5> m_judgementIm;
		std::array<std::unique_ptr<Rect>, 5> m_judgement;

		//NOTE DATA
		std::array<std::vector<Note>, 7> m_notes;//Note data for the song
		std::array<int, 7> m_nextNote = {0,0,0,0,0,0,0};
		std::array<std::vector<std::vector<std::shared_ptr<Rect>>>, 7> m_noteObj;
		std::array<int, 7> m_notesPassed = {0,0,0,0,0,0,0};
		int m_totalNotes = 0;
		int m_totalNotesPassed = 0;

		//OTHER DATA
		std::wstring m_folder = L"../ShuiJamGame/Songs/";
		int m_noteHeight = 25;
		int m_leadin = 2000;//Time miliseconds before the song starts
		float m_spawnPos = 2000;
		float m_t1 = 0;//The intermediate value for the timing buffer lerping
		float m_cSpeed = 550.f;
		float m_curTimePos = 0.f;
		bool m_gameEnded = false;
		float m_totalTransparency = 1.0f;

		std::array<int, 7> m_inputs = //Keyboard inputs
		{g_keyOne, g_keyTwo, g_keyThree , g_keyFour, g_keyFive, g_keySix, g_keySeven};
		std::array<bool, 7> m_pressed = { false, false, false, false, false ,false ,false };
		std::array<bool, 7> m_holdingNote = { false, false, false, false, false ,false ,false };
		std::array<bool, 7> m_failedRelease = { false, false, false, false, false ,false ,false };
		int m_hitPosition = 100;//Pixels above the bottom of the screen determines bar where you hit the note on time
		int m_comboPosition = 470;
		int m_judgePosition = 370;
		bool m_hasHitRecently = false;
		float m_numTimer = 5.0f;
		//Numbers tracking
		int m_recentJudgement = 0;//Most recent judgement hit
		float m_hp = 200;
		int m_combo = 0;
		int m_highestCombo = 0;
		float m_accuracy = 100.000f;
		//Accuracy throughout the song will be calculated as:
		/*((perfCount* perfWeight) + 
		   (greatCount * greatWeight) + 
		   (goodCount * goodWeight) + 
		   (badCount * badWeight) + 
		   (missCount * missWeight)) / m_notesHit;
		*/
		float m_notesHitWeighted = 0.0f;
		float m_notesProcessedWeighted = 0.0f;
		//Judgement counts will be taken over to the results scene
		//[0] = Perfect [1] = Great [2] = Good [3] = Bad [4] = miss
		std::array<int, 5> m_judgementCounts = { 0,0,0,0,0 };
		//Accuracy windows (in milliseconds (perf->bad taken from lunatic rave 2 easy judge))
		//Positive means late, negative means early
		int m_perfWindow = 21;
		int m_greatWindow = 60;
		int m_goodWindow = 120;
		int m_badWindow = 160;
		int m_missWindow = 200;
		//Accuracy weighting (perf = 100%, great = 95% etc.) will be tuned throughout development
		float m_perfWeight = 1.0f;
		float m_greatWeight = 0.97f;
		float m_goodWeight = 0.50f;
		float m_badWeight = 0.25f;
		float m_missWeight = 0.0f;
		
		float m_windowMult = 2.0f;
		float m_gainLossRice = 10.0f;
		float m_gainLossLN = 5.0f;
		/**
		 * \function float lerp(float a, float b, float t)
		 * \param a Start position of the lerp
		 * \param b End position of the lerp
		 * \param t Percentage inbetween start and the end
		 * \return The position of the lerped value
		 */
		float lerp(float a, float b, float t);

		/**
		 * \function void calcJudgementHit(int column).
		 * \brief Carries out the hit judgement function whenever the player presses one of the 7 keys
		 * \param column the corresponding column of the key pressed
		 */
		void calcJudgementHit(int column);

		/**
		 * \function void hitJudgement(int column, int recent, float weight).
		 * \brief Calculates the hit judgement accuracy, increments or resets combo and gains or takes away HP
		 * 
		 * \param column the corresponding column hit
		 * \param recent the most recent judgement hit
		 * \param weight the value it is worth in health and accuracy
		 */
		void hitJudgement(int column, int recent, float weight);

		/**
		 * \function void calcJudgementRelease(int column).
		 * \brief Carries out the hit judgement function whenever the player releases one of the 7 keys
		 * \param column the corresponding column of the key released
		 */
		void calcJudgementRelease(int column);

		/**
		 * \function void releaseJudgement(int column, int recent, float weight).
		 * \brief Calculates the release judgement accuracy, increments or resets combo and gains or takes away HP
		 * 
		 * \param column the corresponding column release
		 * \param recent the most recent judgement release
		 * \param weight the value it is worth in health and accuracy
		 */
		void releaseJudgement(int column, int recent, float weight);
	public:
		GameScene(GLFWwindow* window);
		~GameScene();
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

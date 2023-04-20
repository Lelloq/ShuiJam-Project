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

		std::array<int, 7> m_inputs = //Default keyboard inputs
		{GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C , GLFW_KEY_SPACE, GLFW_KEY_COMMA, GLFW_KEY_PERIOD, GLFW_KEY_SLASH};
		std::array<bool, 7> m_pressed = { false, false, false, false, false ,false ,false };
		std::array<bool, 7> m_holdingNote = { false, false, false, false, false ,false ,false };
		std::array<bool, 7> m_failedRelease = { false, false, false, false, false ,false ,false };
		int m_hitPosition = 100;//Pixels above the bottom of the screen determines bar where you hit the note on time
		int m_comboPosition = 470;
		int m_judgePosition = 370;
		bool m_hasHitRecently = false;
		//Numbers tracking
		int m_recentJudgement = 0;//Most recent judgement hit
		float m_hp = 100;
		int m_combo = 0;
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
		int m_jPerfCount = 0, m_jGreatCount = 0, m_jGoodCount = 0;
		int m_jBadCount = 0, m_jMissCount = 0;
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
		float m_goodWeight = 0.75f;
		float m_badWeight = 0.25f;
		float m_missWeight = 0.0f;
		
		float m_windowMult = 1.3f;
		/**
		 * \function float lerp(float a, float b, float t)
		 * \param a Start position of the lerp
		 * \param b End position of the lerp
		 * \param t Percentage inbetween start and the end
		 * \return The position of the lerped value
		 */
		float lerp(float a, float b, float t);
		void calcJudgementHit(int column);
		void hitJudgement(int column, int recent, float weight);
		void calcJudgementRelease(int column);
		void releaseJudgement(int column, int recent, float weight);
		void play();
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

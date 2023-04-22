/*\file ResultsScene.h*/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "objects/objects.h"
#include <array>

namespace SJ
{
	/*\class ResultsScene
	\brief the result screen of player's performance*/
	class ResultsScene : public Scene
	{
	private:
		//WINDOW
		GLFWwindow* m_window;
		//AUDIO
		AudioDevice* m_device;//!< Sound device
		SoundEffect* m_sfx;//!< Sound effect
		std::unique_ptr<SFXSource> m_source;//!< Sound effect source
		//SFX buffers
		ALuint m_anyKeySound;
		//GRAPHICS
		//Shader
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Shader> m_textShader;
		//Textures
		std::unique_ptr<Texture> m_songIm;
		std::array<std::unique_ptr<Texture>, 5> m_judgementIm;
		std::array<std::unique_ptr<Texture>, 8> m_gradesIm;
		std::unique_ptr<Texture> m_gradesBGIm;
		//Objects
		std::unique_ptr<Rect> m_song;
		std::unique_ptr<Rect> m_gradesBG;
		std::array<std::unique_ptr<Rect>, 5> m_judgement;
		std::array<std::unique_ptr<Rect>, 8> m_grades;

		//Text
		std::unique_ptr<Text> m_percent;
		std::unique_ptr<Text> m_title;
		std::unique_ptr<Text> m_difficulty;
		std::unique_ptr<Text> m_highestCombo;
		std::unique_ptr<Text> m_perfCount;
		std::unique_ptr<Text> m_greatCount;
		std::unique_ptr<Text> m_goodCount;
		std::unique_ptr<Text> m_badCount;
		std::unique_ptr<Text> m_missCount;

		//OTHER DATA
		std::array<float, 7> m_gradeThresholds = {99.75f, 98.0f, 95.0f, 90.0f, 80.0f, 75.0f, 70.0f};
		std::wstring m_folder = L"../ShuiJamGame/Songs/";
		bool m_goBackToSelect = false;
		float m_totalTransparency = 1.0f;
	public:
		ResultsScene(GLFWwindow* window);
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

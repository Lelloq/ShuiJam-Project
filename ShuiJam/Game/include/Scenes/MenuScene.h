/*\file MenuScene.h*/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "objects/objects.h"

namespace SJ
{
	/*\class MenuScene
	\brief the title screen*/
	class MenuScene : public Scene
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
		std::unique_ptr<Shader> m_bgShader;
		std::unique_ptr<Shader> m_titleShader;
		std::unique_ptr<Shader> m_startShader;
		//Textures
		std::unique_ptr<Texture> m_bgIm;
		std::unique_ptr<Texture> m_titleIm;
		std::unique_ptr<Texture> m_startIm;
		//Objects
		std::unique_ptr<Rect> m_bg;
		std::unique_ptr<Rect> m_title;
		std::unique_ptr<Rect> m_start;
		bool m_pressed = false;

		//Animation toggles and values
		float m_intermediate = 0.f;
		bool m_toggle = false;
		float m_toggleValue = 0.f;
		float m_toggleThreshold = 0.5f;
		float m_timer = 0.f;
		float m_sceneTransparency = 1.f;
	public:
		MenuScene(GLFWwindow* window);
		~MenuScene();
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
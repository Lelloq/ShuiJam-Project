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
		std::unique_ptr<Shader> m_bgshader;
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
	public:
		MenuScene(GLFWwindow* window);
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}
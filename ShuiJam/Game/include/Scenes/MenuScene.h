/*\file MenuScene.h*/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/AudioDevice.h"
#include "Audio/SoundEffect.h"
#include <GLFW/glfw3.h>
#include <functional>

//TODO - create vertex class objects to store vertex data for menu images
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
		SJ::AudioDevice* m_device;//!< Audio device
		SJ::SoundEffect* m_sfx;//!< Sound effect
		std::shared_ptr<SJ::SFXSource> m_source;//!< Sound effect source
		//GRAPHICS
		std::shared_ptr<Shader> m_shader;


		ALuint m_anyKeySound;
	public:
		MenuScene() {};
		MenuScene(GLFWwindow* window);
		~MenuScene();
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}
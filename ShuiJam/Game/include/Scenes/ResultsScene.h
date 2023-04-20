/*\file ResultsScene.h*/
#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include "objects/objects.h"

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
		//Textures

		//Objects
	public:
		ResultsScene(GLFWwindow* window);
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}

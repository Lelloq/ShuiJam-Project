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
		std::unique_ptr<Rect> m_grades;
		std::unique_ptr<Rect> m_gradesBG;

		//Text
		std::unique_ptr<Text> m_percent;
		std::unique_ptr<Text> m_highestCombo;
		std::unique_ptr<Text> m_perfCount;
		std::unique_ptr<Text> m_greatCount;
		std::unique_ptr<Text> m_goodCount;
		std::unique_ptr<Text> m_badCount;
		std::unique_ptr<Text> m_missCount;

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

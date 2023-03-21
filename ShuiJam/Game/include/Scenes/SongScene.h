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
#include <GLFW/glfw3.h>

namespace SJ
{
	/**
	 * \class SongScene
	 * \brief The song select scene.
	 */
	class SongScene : public Scene
	{
	private:
		//WINDOW
		GLFWwindow* m_window;
		//AUDIO
		SJ::AudioDevice* m_device;//!< Audio device
		SJ::SoundEffect* m_sfx;//!< Sound effect
		std::unique_ptr<SFXSource> m_SFXscroll;//!< Sound effect source for scrolling
		std::unique_ptr<SFXSource> m_SFXstart;//!< Sound effect source for scrolling
		std::unique_ptr<Music> m_music;//!< Music that plays during song select
		//GRAPHICS
		//Buttons
		std::vector<std::unique_ptr<Button>> m_buttons;//Buttons in song select 11 buttons
		std::vector<std::unique_ptr<Button>> m_sbuttons;//Buttons in settings 2 buttons
		//Shader
		std::unique_ptr<Shader> m_shader;
		//Textures
		std::unique_ptr<Texture> m_songBGIm;
		std::unique_ptr<Texture> m_songSelectIm;
		std::shared_ptr<Texture> m_selectWheelIm;
		//Buttons and images
		std::unique_ptr<Texture> m_logoIm;
		std::unique_ptr<Texture> m_settingsIm;
		std::unique_ptr<Button> m_sButtonmIm;
		std::unique_ptr<Button> m_logo;
		//Objects
		std::unique_ptr<Rect> m_songBG;
		std::unique_ptr<Rect> m_songSelect;
		//Text
		std::unique_ptr<Shader> m_textShader;
		std::unique_ptr<Text> m_text;

		//Values
		std::vector<int> m_positions;//positions of the song wheel

		//GRAPHICS FOR SETTINGS SCREEN

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

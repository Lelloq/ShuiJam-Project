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
		//Shader
		std::unique_ptr<Shader> m_selectShader;
		std::unique_ptr<Shader> m_songBGShader;
		std::unique_ptr<Shader> m_wheelShader;
		//Textures
		std::unique_ptr<Texture> m_songBGIm;
		std::unique_ptr<Texture> m_songSelectIm;
		std::shared_ptr<Texture> m_selectWheelIm;
		//Objects
		std::unique_ptr<Rect> m_songBG;
		std::unique_ptr<Rect> m_songSelect;
		std::unique_ptr<Rect> m_songWheelPart;

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

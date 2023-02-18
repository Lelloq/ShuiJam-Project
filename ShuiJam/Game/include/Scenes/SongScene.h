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
		//AUDIO
		SJ::AudioDevice* m_device;//!< Audio device
		SJ::SoundEffect* m_sfx;//!< Sound effect
		std::shared_ptr<SJ::SFXSource> m_source;//!< Sound effect source
	public:
		SongScene();
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}

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
		//AUDIO
		SJ::AudioDevice* m_device;//!< Audio device
		SJ::SoundEffect* m_sfx;//!< Sound effect
		std::shared_ptr<SJ::SFXSource> m_source;//!< Sound effect source
		//GRAPHICS
		std::shared_ptr<Texture> m_bg;
		std::shared_ptr<Texture> m_title;
		std::shared_ptr<Texture> m_text;
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Rect> m_titleBG;

		ALuint m_anyKeySound;
		bool m_pressed = false;
	public:
		MenuScene();
		void Update(float dt);
		void Render();
		void getKey(int key, int scancode, int action, int mods) override;
		void getMouseButton(int button, int action, int mods) override;
		void getScroll(double xoffset, double yoffset) override;
		void fileDrop(int count, const char** paths) override;
	};
}
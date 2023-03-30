/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include <future>
#include "Audio/Music.h"

SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;

void main()
{
	SJ::WindowManager gameWindow = SJ::WindowManager(SCR_WIDTH, SCR_HEIGHT, 0, 0, "ShuiJam");
	std::unique_ptr<SJ::MenuScene> menu = std::make_unique<SJ::MenuScene>(gameWindow.getWindow());
	std::unique_ptr<SJ::SongScene> songSelect;
	std::unique_ptr<SJ::GameScene> game;
	bool isPlaying = false;//There so that it creates the game scene once in the game loop scope

	audioDevice = SJ::AudioDevice::get();
	soundEffect = SJ::SoundEffect::get();

	//SJ::Music m(SJFOLDER + SOUNDS + "parallax.mp3");
	//std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);

	auto isExtracted = std::async(std::launch::async, SJ::FileExtractor::extractFiles);
	//m.Play();

	SJ::Scene::setWindow(gameWindow.getWindow());
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.beginFrame();
		if (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && g_CurrentScene == "song_select") glfwSetWindowShouldClose(gameWindow.getWindow(), true);
		//std::cout << m.getTimePosition() << "\n";
		if(g_CurrentScene == "title")
		{
			SJ::Scene::setInputCallbacks(menu.get());
			menu->Update(gameWindow.getDeltatime());
			menu->Render();
		}
		else if(g_CurrentScene == "song_select")
		{
			isPlaying = false;
			if (songSelect == nullptr) songSelect = std::make_unique<SJ::SongScene>(gameWindow.getWindow());
			SJ::Scene::setInputCallbacks(songSelect.get());
			songSelect->Update(gameWindow.getDeltatime());
			songSelect->Render();
		}
		else if(g_CurrentScene == "game")
		{
			if(!isPlaying)
			{
				isPlaying = true;
				game = std::make_unique<SJ::GameScene>(gameWindow.getWindow());
				SJ::Scene::setInputCallbacks(game.get());
			}
		}
		else if(g_CurrentScene == "result")
		{
			isPlaying = false;
			if (game != nullptr) { game.reset(); }
		}
		//std::cout << SJ::isFutureReady(isExtracted) << "\n";
		//m.Update();
		gameWindow.Swap();
	}

	isExtracted.wait();
	gameWindow.Shutdown();
}



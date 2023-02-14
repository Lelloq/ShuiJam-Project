/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include <future>
#include "Audio/Music.h"

SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;

void setup()
{
    audioDevice = SJ::AudioDevice::get();
	soundEffect = SJ::SoundEffect::get();
}

void main()
{
	SJ::WindowManager gameWindow = SJ::WindowManager(1280, 720, 0, 0, "ShuiJam");
	SJ::MenuScene menu = SJ::MenuScene(gameWindow.getWindow());

	setup();

	std::string currentScene = "menu";

	SJ::Music m(SJFOLDER + SOUNDS + "parallax.mp3");
	std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);

	auto isExtracted = std::async(std::launch::async, SJ::FileExtractor::extractFiles);
	m.Play();

	SJ::Scene::setWindow(gameWindow.getWindow());
	SJ::Scene::setInputCallbacks(menu);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.beginFrame();
		//std::cout << m.getTimePosition() << "\n";
		menu.Render();
		//std::cout << SJ::isFutureReady(isExtracted) << "\n";
		m.Update();
		gameWindow.Swap();
	}

	isExtracted.wait();
	gameWindow.Shutdown();
}



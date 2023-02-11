/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include <future>
#include "Audio/Music.h"

SJ::WindowManager gameWindow;
SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;

void setup()
{
	gameWindow = SJ::WindowManager(1280, 720, 0, 0, "ShuiJam");
    audioDevice = SJ::AudioDevice::get();
	soundEffect = SJ::SoundEffect::get();
}

void main()
{
	setup();
	std::string currentScene = "menu";

	SJ::MenuScene menu = SJ::MenuScene(gameWindow.getWindow());

	SJ::Music m(SJFOLDER + SOUNDS + "parallax.mp3");
	m.Play();
	std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);
	//ALuint testsfx = soundEffect->addSFX(SJFOLDER + SOUNDS + "parallax.mp3");

	auto isExtracted = std::async(std::launch::async, SJ::FileExtractor::extractFiles);

	//SFX->Play(testsfx);
	gameWindow.Start();

	SJ::Scene::setWindow(gameWindow.getWindow());
	SJ::Scene::setInputCallbacks(menu);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.beginFrame();
		//std::cout << m.getTimePosition() << std::endl;
		std::cout << SJ::isFutureReady(isExtracted) << "\n";
		m.Update();
		gameWindow.Swap();
	}

	isExtracted.wait();
	gameWindow.Shutdown();
}



/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
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

	SJ::MenuScene menu = SJ::MenuScene(gameWindow.getWindow());

	SJ::Music m(SJFOLDER + SOUNDS + "parallax.mp3");
	m.Play();
	std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);
	//ALuint testsfx = soundEffect->addSFX(SJFOLDER + SOUNDS + "parallax.mp3");

	SJ::FileExtractor::extractFiles();

	//SFX->Play(testsfx);
	gameWindow.Start();

	SJ::Scene::setWindow(gameWindow.getWindow());
	SJ::Scene::setInputCallbacks(menu);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.Update();
		std::cout << m.getTimePosition() << std::endl;
		m.Update();
	}

	gameWindow.Shutdown();
}
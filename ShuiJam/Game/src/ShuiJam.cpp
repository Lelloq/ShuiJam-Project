/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include "Audio/Music.h"

//#include "../Tests/include/sjtester.h"
const bool RUNTESTING = false;

SJ::WindowManager gameWindow;
SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;
void menuInputCallbacks(SJ::MenuScene scene);
void setup()
{
	gameWindow = SJ::WindowManager(1280, 720, 0, 0, "ShuiJam");
    audioDevice = SJ::AudioDevice::get();
	soundEffect = SJ::SoundEffect::get();
}

void main()
{
	//If I'm not running google test then I'll create the devices and window
	if(RUNTESTING)
	{
		//SJTester::runtests();
		return;
	}
	else
	{
		setup();
	}

	SJ::MenuScene menu = SJ::MenuScene(gameWindow.getWindow());

	SJ::Music m("../ShuiJamGame/Sounds/audio.mp3");
	m.Play();
	std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);
	//ALuint testsfx = soundEffect->addSFX("../ShuiJamGame/Sounds/audio.mp3");

	SJ::FileExtractor* fe = SJ::FileExtractor::get();
	fe->extractFiles();

	//SFX->Play(testsfx);
	gameWindow.Start();
	menuInputCallbacks(menu);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.Update();
		m.Update();
	}

	gameWindow.Shutdown();
}

void menuInputCallbacks(SJ::MenuScene scene)
{
	glfwSetWindowUserPointer(gameWindow.getWindow(), &scene);

	glfwSetKeyCallback(gameWindow.getWindow(), [](GLFWwindow* win, int key, int scancode, int action, int mods) 
		{
			static_cast<SJ::MenuScene*>(glfwGetWindowUserPointer(win))->getKey(key, action);
		});
}
/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include "Audio/Music.h"

//#include "../Tests/include/sjtester.h"
const bool RUNTESTING = false;

SJ::WindowManager gameWindow;
SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;
void inputCallbacks(SJ::Scene& scene);
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

	SJ::Music m(SJFOLDER + SOUNDS + "audio.mp3");
	m.Play();
	std::shared_ptr<SJ::SFXSource> SFX(new SJ::SFXSource);
	//ALuint testsfx = soundEffect->addSFX("../ShuiJamGame/Sounds/audio.mp3");

	SJ::FileExtractor* fe = SJ::FileExtractor::get();
	fe->extractFiles();

	//SFX->Play(testsfx);
	gameWindow.Start();
	inputCallbacks(menu);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.Update();
		m.Update();
	}

	gameWindow.Shutdown();
}

void inputCallbacks(SJ::Scene& scene)
{
	glfwSetWindowUserPointer(gameWindow.getWindow(), &scene);

	glfwSetKeyCallback(gameWindow.getWindow(), [](GLFWwindow* win, int key, int scancode, int action, int mods) 
		{
			static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getKey(key, scancode, action, mods);
		});

	glfwSetMouseButtonCallback(gameWindow.getWindow(), [](GLFWwindow* win, int button, int action, int mods)
		{
			static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getMouseButton(button, action, mods);
		});

	glfwSetScrollCallback(gameWindow.getWindow(), [](GLFWwindow* win, double xoffset, double yoffset) 
		{
			static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getScroll(xoffset, yoffset);
		});

	glfwSetDropCallback(gameWindow.getWindow(), [](GLFWwindow* win, int count, const char** paths)
		{
			static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->fileDrop(count, paths);
		});
}
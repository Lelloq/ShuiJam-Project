/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>

SJ::WindowManager gameWindow = SJ::WindowManager(1280, 720, 0, 0, "ShuiJam");
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void main()
{
	SJ::AudioDevice* audioDevice = SJ::AudioDevice::get();
	SJ::SoundEffect* soundEffect = SJ::SoundEffect::get();
	ALuint testsfx = soundEffect->addSFX("../SJAssets/Sounds/DragonLady.ogg");
	SJ::SFXSource SFX = SJ::SFXSource();

	SJ::FileExtractor* fe = SJ::FileExtractor::get();
	fe->extractFiles();

	SFX.Play(testsfx);
	gameWindow.Start();
	glfwSetKeyCallback(gameWindow.getWindow(), key_callback);
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.Update();
	}
	gameWindow.Shutdown();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		std::cout << "E";
	}
	if (key == GLFW_KEY_E && action == GLFW_REPEAT)
		std::cout << "e";
}
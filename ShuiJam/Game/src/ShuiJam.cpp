/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include "bitextractor.hpp"
#include "bitexception.hpp"
#include <iostream>

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void main()
{

	AudioDevice* audioDevice = AudioDevice::get();
	ALuint testsfx = SoundEffect::get()->addSFX("../SJAssets/Sounds/hitclap.wav");
	SFXSource SFX;

	SFX.Play(testsfx);

	gameWindow->Start();
	glfwSetKeyCallback(gameWindow->getWindow(), key_callback);
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Update();
	}
	gameWindow->Shutdown();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		std::cout << "E";
	if (key == GLFW_KEY_E && action == GLFW_REPEAT)
		std::cout << "e";
}
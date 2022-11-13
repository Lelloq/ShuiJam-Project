/*\file MenuScene.h*/
#pragma once
#include "Renderer.h"
#include "Audio/AudioDevice.h"
#include "Audio/SoundEffect.h"
#include <GLFW/glfw3.h>

class MenuScene
{
private:
	GLFWwindow* m_window;
	//TODO - create vertex class objects to store vertex data for menu images
public:
	MenuScene(GLFWwindow* window);
	~MenuScene();
	void getKey(GLFWwindow* window, int key, int scancode, int action, int mods);
};
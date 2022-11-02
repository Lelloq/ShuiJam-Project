/*\file InputManager.h*/
#pragma once
#include <GLFW/glfw3.h>

/*\class InputManager*/
class InputManager
{
public:
	virtual void SetCallback(GLFWwindow* window, GLFWkeyfun callback);
};
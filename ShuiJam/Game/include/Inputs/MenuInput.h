/*\file MenuInput*/
#pragma once
#include "InputManager.h"

class MenuInput : public InputManager
{
private:
	GLFWwindow* m_window;
public:
	MenuInput(GLFWwindow* window);
};

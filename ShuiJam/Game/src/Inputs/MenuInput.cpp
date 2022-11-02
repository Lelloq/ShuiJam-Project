#include "Inputs/MenuInput.h"
#include <iostream>

MenuInput::MenuInput(GLFWwindow* window) : m_window(window)
{
	glfwSetKeyCallback(window, keyPressedEvent);
}
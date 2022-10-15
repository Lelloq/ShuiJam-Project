#include "pch.h"
#include "WindowManager.h"

WindowManager gameWindow = WindowManager(1280,720,0);

void main()
{
	while(!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.Loop();
	}
	gameWindow.Shutdown();
}
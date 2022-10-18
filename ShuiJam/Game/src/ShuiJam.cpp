#include "pch.h"
#include "WindowManager.h"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, false, "ShuiJam");
//WindowManager* launcherWindow = new WindowManager(500,500,1, "ShuiJam Launcher");

void main()
{
	gameWindow->Start();
	while (!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Loop();
	}
}
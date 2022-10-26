#include "pch.h"
#include "ShuiJam.h"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");

void main()
{
	gameWindow->Start();
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Loop();
	}
	gameWindow->Shutdown();
}
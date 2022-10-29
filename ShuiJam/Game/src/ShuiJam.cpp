#include "pch.h"
#include "ShuiJam.h"
#include "bitextractor.hpp"
#include "bitexception.hpp"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");

using namespace bit7z;

void main()
{
	gameWindow->Start();
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Loop();
	}
	gameWindow->Shutdown();
}
/*\file ShuiJam.cpp*/
#include "pch.h"
#include "ShuiJam.h"
#include "bitextractor.hpp"
#include "bitexception.hpp"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");

void main()
{
	gameWindow->Start();
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Update();
	}
	gameWindow->Shutdown();
}
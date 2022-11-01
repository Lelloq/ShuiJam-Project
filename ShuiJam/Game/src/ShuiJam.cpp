/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include "bitextractor.hpp"
#include "bitexception.hpp"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");
AudioDevice* audioDevice = AudioDevice::get();
ALuint testsfx = SoundEffect::get()->addSFX("../SJAssets/Sounds/hitclap.wav");
SFXSource SFX;

void main()
{

	SFX.Play(testsfx);

	gameWindow->Start();
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Update();
	}
	gameWindow->Shutdown();
}
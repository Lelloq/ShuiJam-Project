#include "pch.h"
#include "ShuiJam.h"
#include "bitextractor.hpp"
#include "bitexception.hpp"

WindowManager* gameWindow = new WindowManager(1280, 720, 0, 0, "ShuiJam");

using namespace bit7z;

void main()
{
	try
	{
		Bit7zLibrary lib{ L"../vendor/bit7z/dll/7z.dll" };
		BitExtractor ext{ lib, BitFormat::Zip };

		ext.extract(L"../SJAssets/Input/1114344 Sangatsu no Phantasia - Pastel Rain.osz", L"../SJAssets/Songs/");
	}
	catch (const BitException& ex)
	{
		std::cout << ex.what();
	}

	gameWindow->Start();
	while(!glfwWindowShouldClose(gameWindow->getWindow()))
	{
		gameWindow->Loop();
	}
	gameWindow->Shutdown();
}
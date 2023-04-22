/*\file ShuiJam.cpp*/
#include "ShuiJam.h"
#include <iostream>
#include <future>
#include "Audio/Music.h"

SJ::AudioDevice* audioDevice;
SJ::SoundEffect* soundEffect;

void main()
{
	//Create window and prepare the scenes
	SJ::WindowManager gameWindow = SJ::WindowManager(SCR_WIDTH, SCR_HEIGHT, false, false, "ShuiJam");
	std::unique_ptr<SJ::MenuScene> menu = std::make_unique<SJ::MenuScene>(gameWindow.getWindow());
	std::unique_ptr<SJ::SongScene> songSelect;
	std::unique_ptr<SJ::GameScene> game;
	std::unique_ptr<SJ::ResultsScene> result;

	audioDevice = SJ::AudioDevice::get();
	soundEffect = SJ::SoundEffect::get();

	//Extract any files in the input folder
	auto isExtracted = std::async(std::launch::async, SJ::FileExtractor::extractFiles);
	//m.Play();

	//Set the input callbacks to the main menu
	SJ::Scene::setWindow(gameWindow.getWindow());
	SJ::Scene::setInputCallbacks(menu.get());
	while (!glfwWindowShouldClose(gameWindow.getWindow()))
	{
		gameWindow.beginFrame();
		//Close the game if the player presses escape and is in the song selection
		if (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && g_CurrentScene == "song_select") glfwSetWindowShouldClose(gameWindow.getWindow(), true);
		if (g_CurrentScene == "title")
		{
			menu->Update(gameWindow.getDeltatime());
			menu->Render();
		}
		else if (g_CurrentScene == "song_select")
		{
			//Cleanup the menu and result screen objects if they haven't already
			if (menu != nullptr) menu.reset();
			if (result != nullptr) 
			{
				result.reset(); 
				//Since its going back to the song select which isn't cleared since you want to save the position of the song wheel
				//I have to set input callbacks back to the song select
				SJ::Scene::setInputCallbacks(songSelect.get());
			}
			//Create the scene or render it if it has been created already
			if (songSelect == nullptr)
			{
				songSelect = std::make_unique<SJ::SongScene>(gameWindow.getWindow());
				SJ::Scene::setInputCallbacks(songSelect.get());
			}
			else
			{
				songSelect->Update(gameWindow.getDeltatime());
				songSelect->Render();
			}
		}
		else if (g_CurrentScene == "game")
		{
			if (game == nullptr)
			{
				game = std::make_unique<SJ::GameScene>(gameWindow.getWindow());
				SJ::Scene::setInputCallbacks(game.get());
			}
			else
			{
				game->Update(gameWindow.getDeltatime());
				game->Render();
			}
		}
		else if (g_CurrentScene == "result")
		{
			if (game != nullptr) { game.reset(); }
			if (result == nullptr)
			{
				result = std::make_unique<SJ::ResultsScene>(gameWindow.getWindow());
				SJ::Scene::setInputCallbacks(result.get());
			}
			else
			{
				result->Update(gameWindow.getDeltatime());
				result->Render();
			}
		}
		gameWindow.Swap();
	}

	isExtracted.wait();
	gameWindow.Shutdown();
}
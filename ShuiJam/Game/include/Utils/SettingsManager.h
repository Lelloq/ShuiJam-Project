/*****************************************************************//**
 * \file   SettingsManager.h
 * \brief  Handles file saving using json files
 * 
 * \date   April 2023
 *********************************************************************/
#pragma once
#include <json.hpp>
#include "Utils/Properties.h"
#include "Utils/Settings.h"
#include <filesystem>
#include <fstream>

namespace SJ
{
	class SaveManager
	{
	private:
		inline static const std::wstring m_settingsJson = L"../ShuiJamGame/settings.json";
	public:
		/**
		 * \function static void Init().
		 * \brief Opens the json file or creates on if it doesn't exist
		 */
		static void Init();
		/**
		 * \function static void Load().
		 * \brief Loads the json file data into the global variables inside Settings.h
		 */
		static void Load();
		/**
		 * \function static void Save().
		 * \brief Saves the global variables inside Settings.h to the json file
		 */
		static void Save();
	};
}

/*****************************************************************//**
 * \file   SaveManager.cpp
 * \brief  Handles file saving using json files
 * 
 * \date   April 2023
 *********************************************************************/
#include "Utils/SettingsManager.h"

using json = nlohmann::json;

namespace SJ
{
	namespace fs = std::filesystem;
	void SaveManager::Init()
	{
		if(!fs::exists(m_settingsJson))
		{
			std::ofstream file = std::ofstream(m_settingsJson);
			file.close();
		}
	}

	void SaveManager::Load()
	{

	}

	void SaveManager::Save()
	{

	}
}

/*****************************************************************//**
 * \file   SettingsManager.cpp
 * \brief  Handles file saving using json files
 * 
 * \date   April 2023
 *********************************************************************/
#include "Utils/SettingsManager.h"
#include <iostream>

using json = nlohmann::json;

namespace SJ
{
	namespace fs = std::filesystem;
	void SettingsManager::Init()
	{
		if(!fs::exists(m_settingsJson))
		{
			std::ofstream file = std::ofstream(m_settingsJson);
			Save();
			file.close();
		}
	}

	void SettingsManager::Load()
	{
		std::ifstream jsonf = std::ifstream(m_settingsJson);
		json settings = json::parse(jsonf);

		g_offset = settings.at(0).at(1).at(1);//Offset (offsets: -> offset -> value)
		g_hitposition = settings.at(0).at(2).at(1);//Hitposition (offsets: -> hitposition -> value)

		g_keyOne = settings.at(1).at(1).at(1);//Keyone (keys -> keyone -> value)
		g_keyTwo = settings.at(1).at(2).at(1);//Keytwo
		g_keyThree = settings.at(1).at(3).at(1);//Keythree
		g_keyFour = settings.at(1).at(4).at(1);//Keyfour
		g_keyFive = settings.at(1).at(5).at(1);//Keyfive
		g_keySix = settings.at(1).at(6).at(1);//Keysix
		g_keySeven = settings.at(1).at(7).at(1);//Keyseven

		g_width = settings.at(2).at(1);//width (width -> value)
		g_height = settings.at(3).at(1);//height
		g_volume = settings.at(4).at(1);//volume
		g_fullscreen = settings.at(5).at(1);//fullscreen window
		g_scrollspeed = settings.at(6).at(1);//Note scrollspeed
	}

	void SettingsManager::Save()
	{
		std::ofstream file = std::ofstream(m_settingsJson);
		json values = 
		{
			{"offsets:", 
				{"offset:", g_offset},
				{"hitposition:", g_hitposition},
			},
			{"Keys:", 
				{"keyOne:", g_keyOne},
				{"keyTwo:", g_keyTwo},
				{"keyThree:", g_keyThree},
				{"keyFour:", g_keyFour},
				{"keyFive:", g_keyFive},
				{"keySix:", g_keySix},
				{"keySeven:", g_keySeven},
			},
			{"width:", g_width},
			{"height:", g_height},
			{"volume:", g_volume},
			{"fullscreen:", g_fullscreen},
			{"scrollspeed:", g_scrollspeed}
		};
		file << values.dump();
		file.close();
	}
}

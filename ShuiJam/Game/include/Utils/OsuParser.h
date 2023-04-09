/*\file OsuParser.h
\brief parses the .osu file*/
#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <array>

namespace SJ
{
	/*
	* \struct Note
	* \brief Note data for a note object to be used in the game
	*/
	struct Note
	{
		int column;
		int timingPoint;
		int releasePoint = 0;
	};

	/**
	 * \class OsuParser.
	 * \brief Parses the osu file when given the mapset and the map difficulty
	 */
	class OsuParser
	{
	private:
		inline static const std::wstring m_songsFolder = L"../ShuiJamGame/Songs/";
	public:
		static std::array<std::vector<Note>, 7> parse(std::wstring& dirName, std::wstring& osuName);
	};
}

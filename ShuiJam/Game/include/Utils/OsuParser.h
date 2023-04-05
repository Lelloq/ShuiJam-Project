/*\file OsuParser.h
\brief parses the .osu file*/
#pragma once
#include <vector>
#include <filesystem>
#include <fstream>

/*TODO
-Figure out the structure of the osu file
-Separate the metadata from the timing data
-Convert timing points into note positions
-Filter out unnecessary options in the osu file
-Find a built in library that goes through the file line by line*/
namespace SJ
{
	struct Note
	{
		int column;
		int timingPoint;
		int releasePoint = 0;
	};

	class OsuParser
	{
	private:
		const std::wstring m_songsFolder = L"../ShuiJamGame/Songs/";
		std::vector<Note> m_notes;
	public:
		static std::vector<Note> parse();
	};
}

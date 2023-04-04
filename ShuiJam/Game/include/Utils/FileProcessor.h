/*\file FileProcessor.h
\brief Processes the files in the song and puts them into a file for the parser to use put into a .db file*/
#pragma once

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

/*TODO
-Filter out non 7k maps which can be found through speicfic .osu file settings
-Use sqlite to create a db file and move these strings into there:
-.osu filepath
-beatmap ID
-bg image path*/
namespace SJ
{
	struct Songdata
	{
		std::wstring artist;
		std::wstring title;
		std::wstring dirPath, osuPath;
		std::wstring background, audio;
	};

	/*\class FileProcessor.h
	\brief Process files in the song list, removing non 7k maps and creating a filepath to the .osu file*/
	class FileProcessor
	{
	private:
		sqlite3* m_db;
		sqlite3_stmt* m_stmt;
		const std::wstring m_songsFolder = L"../ShuiJamGame/Songs/";
		const std::string m_dbLocation = "../ShuiJamGame/shuijam.db";
		unsigned m_lastID = 0;
		void setLastID();
	public:
		FileProcessor();//!<Constructor that init sqlite3 library
		~FileProcessor();//!<Destructor that stops sqlite3 library
		void ProcessFiles();//!<Filters out non 7key mode .osu files
		void reloadSongs();//!<Loads the .osu files into the database created
		Songdata retrieveSong(int row);//!<Retrieves the song data for a specific row
	};
}
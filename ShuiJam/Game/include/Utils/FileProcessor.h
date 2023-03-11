/*\file FileProcessor.h
\brief Processes the files in the song and puts them into a file for the parser to use put into a .db file*/
#pragma once

#include <sqlite3.h>
#include <filesystem>
#include <string>

/*TODO
-Filter out non 7k maps which can be found through speicfic .osu file settings
-Use sqlite to create a db file and move these strings into there:
-.osu filepath
-beatmap ID
-bg image path*/
namespace SJ
{
	/*\class FileProcessor.h
	\brief Process files in the song list, removing non 7k maps and creating a filepath to the .osu file*/
	class FileProcessor
	{

	};
}
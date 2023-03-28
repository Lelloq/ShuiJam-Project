#include "Utils/FileProcessor.h"

namespace SJ
{
	FileProcessor::FileProcessor()
	{
		sqlite3_open(m_dbLocation.c_str(), &m_db);
	}

	FileProcessor::~FileProcessor()
	{
		sqlite3_close(m_db);
	}

	void FileProcessor::ProcessFiles()
	{
		/*to filter out the non 7k maps, in the .osu file mode is 3 and circlesize is 7*/
		namespace fs = std::filesystem;
		for (auto& entry : fs::directory_iterator(m_songsFolder))
		{
			int mapCount = 0;
			if (!entry.is_directory()) continue;
			std::cout << fs::relative(entry, m_songsFolder) << "\n";
			for (auto& osu : fs::directory_iterator(entry))
			{
				if (osu.path().extension() != ".osu") continue;
				mapCount++;
				//std::cout << osu << "\n";
				std::fstream file;
				file.open(osu);
				std::string mode, keymode;
				//In the osu file the "circlesize" which is the keymode is after the game mode since osu has 4 modes
				//After it gets the "circlesize" it breaks the loop
				for (std::string line; std::getline(file, line); )
				{
					if (line.find("Mode") != std::string::npos)
					{
						mode = line;
					}
					if (line.find("CircleSize") != std::string::npos)
					{
						keymode = line;
						file.close();
						break;
					}
				}
				if (mode != "Mode: 3" || keymode != "CircleSize:7")
				{
					fs::remove(osu);
					mapCount--;
				}
			}
			if (mapCount == 0) fs::remove_all(entry);
		}
	}

	void FileProcessor::reloadSongs()
	{
		std::string songsTable = "Songs";

		std::string deleteCommand =
			"DELETE FROM " + songsTable;

		std::string createCommand = 
			"CREATE TABLE IF NOT EXISTS Songs("
			"ID Int NOT NULL,"
			"Artist Text(65535),"
			"Path Text(65535),"
			"OSU Text(65535),"
			"Background Text(65535),"
			"Audio Text(65535) );";

		int del = 0;
		char* delErr;
		del = sqlite3_exec(m_db, deleteCommand.c_str(), NULL, 0, &delErr);
		if(del != SQLITE_OK)
		{
			std::cout << "Error deleting table" << "\n";
		}

		int create = 0;
		char* createErr;
		create = sqlite3_exec(m_db, createCommand.c_str(), NULL, 0, &createErr);
		if(create != SQLITE_OK)
		{
			std::cout << "Error creating table" << "\n";
		}

		namespace fs = std::filesystem;
		std::wstring insertCommand =
		L" INSERT INTO Songs (ID,Artist,Path,OSU,Background,Audio) "
		"VALUES(?,?,?,?,?,?)";

		int insert;
		sqlite3_stmt* stmt;
		insert = sqlite3_prepare16_v2(m_db, insertCommand.c_str(), -1, &stmt, nullptr);

		for (auto& entry : fs::directory_iterator(m_songsFolder))
		{
			if (!entry.is_directory()) continue;
			std::wstring dirPath = fs::relative(entry, m_songsFolder);
			for (auto& osu : fs::directory_iterator(entry))
			{
				std::wstring beatmapID, artist, osuPath, bgPath, audioPath;
				std::wfstream file;
				if (osu.path().extension() != ".osu") continue;

				osuPath = fs::relative(entry, m_songsFolder);
				std::wcout << osuPath << "\n";

				file.open(osu);
				int bgCounter = 0;
				for(std::wstring line; std::getline(file, line); )
				{
					if(line.find(L"BeatmapID:") != std::wstring::npos)
					{
						std::wstring temp = L"BeatmapID:";
						line.erase(0, temp.size());
						beatmapID = line;
						std::wcout << beatmapID << "\n";
					}
					if(line.find(L"AudioFilename: ") != std::wstring::npos)
					{
						std::wstring temp = L"AudioFilename: ";
						line.erase(0, temp.size());
						audioPath = line;
						std::wcout << audioPath << "\n";
					}
					if(line.find(L"Artist:") != std::wstring::npos)
					{
						std::wstring temp = L"Artist:";
						line.erase(0, temp.size());
						artist = line;
						std::wcout << artist << "\n";
					}
					if(line.find(L"//Background and Video events") != std::wstring::npos)
					{
						bgCounter++;
					}
					else if(bgCounter == 1)
					{
						bgCounter = 0;
						int start = line.find_first_of(L"\"");
						int end = line.find_last_of(L"\"");
						bgPath = line.substr(start+1, end-1 - start);
						std::wcout << bgPath << "\n";
					}
				}
				file.close();

				sqlite3_bind_text16(stmt, 1, beatmapID.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 2, artist.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 3, dirPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 4, osuPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 5, bgPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 6, audioPath.c_str(), -1, SQLITE_STATIC);

				int val = sqlite3_step(stmt);
				if(val != SQLITE_DONE)
				{
					std::cout << "Error inserting data" << "\n";
				}
			}
		}
		sqlite3_finalize(stmt);
	}
}
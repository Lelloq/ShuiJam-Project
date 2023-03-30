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
			"mapID Int NOT NULL,"
			"Artist Text(65535),"
			"Title Text(65535),"
			"Path Text(65535),"
			"OSU Text(65535),"
			"Background Text(65535),"
			"Audio Text(65535) );";

		int create = 0;
		char* createErr;
		create = sqlite3_exec(m_db, createCommand.c_str(), NULL, 0, &createErr);
		if(create != SQLITE_OK)
		{
			std::cout << "Error creating table" << "\n";
		}

		int del = 0;
		char* delErr;
		del = sqlite3_exec(m_db, deleteCommand.c_str(), NULL, 0, &delErr);
		if(del != SQLITE_OK)
		{
			std::cout << "Error clearing table" << "\n";
		}

		namespace fs = std::filesystem;
		std::wstring insertCommand =
		L" INSERT INTO Songs (ID,mapID,Artist,Title,Path,OSU,Background,Audio) "
		"VALUES(?,?,?,?,?,?,?,?)";

		sqlite3_stmt* stmt;
		sqlite3_prepare16_v2(m_db, insertCommand.c_str(), -1, &stmt, nullptr);

		int id = 0;
		for (auto& entry : fs::directory_iterator(m_songsFolder))
		{
			if (!entry.is_directory()) continue;
			std::wstring dirPath = fs::relative(entry, m_songsFolder);
			for (auto& osu : fs::directory_iterator(entry))
			{
				int beatmapID = -1;
				std::wstring artist, title, osuPath, bgPath, audioPath;
				std::wfstream file;
				if (osu.path().extension() != ".osu") continue;

				osuPath = fs::relative(osu, m_songsFolder);
				osuPath.erase(0, osuPath.find_first_of(L"\\") + 1);
				std::wcout << osuPath << "\n";

				file.open(osu);
				int bgCounter = 0;
				for(std::wstring line; std::getline(file, line); )
				{
					if(line.find(L"BeatmapID:") != std::wstring::npos)
					{
						std::wstring temp = L"BeatmapID:";
						line.erase(0, temp.size());
						beatmapID = std::stoi(line);
						std::wcout << beatmapID << "\n";
					}
					if(line.find(L"TitleUnicode:") != std::wstring::npos)
					{
						std::wstring temp = L"TitleUnicode:";
						line.erase(0, temp.size());
						title = line;
						std::wcout << title << "\n";
					}
					if(line.find(L"AudioFilename: ") != std::wstring::npos)
					{
						std::wstring temp = L"AudioFilename: ";
						line.erase(0, temp.size());
						audioPath = line;
						std::wcout << audioPath << "\n";
					}
					if(line.find(L"ArtistUnicode:") != std::wstring::npos)
					{
						std::wstring temp = L"ArtistUnicode:";
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

				sqlite3_bind_int(stmt, 1, id);
				sqlite3_bind_int(stmt, 2, beatmapID);
				sqlite3_bind_text16(stmt, 3, artist.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 4, title.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 5, dirPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 6, osuPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 7, bgPath.c_str(), -1, SQLITE_STATIC);
				sqlite3_bind_text16(stmt, 8, audioPath.c_str(), -1, SQLITE_STATIC);

				int val = sqlite3_step(stmt);
				if(val != SQLITE_DONE)
				{
					std::cout << "Error inserting data" << "\n";
				}
			}
		}
		sqlite3_finalize(stmt);
	}

	Songdata FileProcessor::retrieveSong(int row)
	{
		Songdata data;
		std::wstring selectCommand =
			L"SELECT * FROM Songs WHERE ID = ?";

		sqlite3_stmt* stmt;
		sqlite3_prepare16_v2(m_db, selectCommand.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, row);
		sqlite3_step(stmt);

		data.artist = (wchar_t*)sqlite3_column_text16(stmt, 2);
		data.title = (wchar_t*)sqlite3_column_text16(stmt, 3);
		data.dirPath = (wchar_t*)sqlite3_column_text16(stmt, 4);
		data.osuPath = (wchar_t*)sqlite3_column_text16(stmt, 5);
		data.background = (wchar_t*)sqlite3_column_text16(stmt, 6);
		data.audio = (wchar_t*)sqlite3_column_text16(stmt, 7);

		sqlite3_finalize(stmt);
		return data;
	}
}
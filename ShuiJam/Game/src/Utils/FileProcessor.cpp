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
		for(auto& entry : fs::directory_iterator(m_songsFolder))
		{
			int mapCount = 0;
			if(entry.is_directory())
			{
				for(auto& osu : fs::directory_iterator(entry))
				{
					if(osu.path().extension() == ".osu") 
					{
						mapCount++;
						//std::cout << osu << "\n";
						std::fstream file;
						file.open(osu);
						std::string mode, keymode;
						//In the osu file the "circlesize" which is the keymode is after the game mode since osu has 4 modes
						//After it gets the "circlesize" it breaks the loop
						for(std::string line ; std::getline(file, line); )
						{
							if (line.find("Mode") != std::string::npos)
							{
								mode = line;
							}
							if(line.find("CircleSize") != std::string::npos)
							{
								keymode = line;
								break;
							}
						}
						file.close();
						if(mode != "Mode: 3" || keymode != "CircleSize:7")
						{
							fs::remove(osu);
							mapCount--;
						}
					}
				}
			}
			if (mapCount == 0) fs::remove_all(entry);
		}
	}
}

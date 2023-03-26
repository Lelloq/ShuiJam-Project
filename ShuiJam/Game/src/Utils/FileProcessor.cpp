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
		namespace fs = std::filesystem;
		for(auto& entry : fs::directory_iterator(m_songsFolder))
		{
			if(entry.is_directory())
			{
				for(auto& osu : fs::directory_iterator(entry))
				{
					std::cout << osu << "\n";
				}
			}
		}
	}
}

/*\file FileExtractor.cpp*/
#include "Utils/FileExtractor.h"
#include "Utils/FileProcessor.h"

//TODO
//Filter out the non 7 key .osu files within each file
//Delete files that are not the main audio file

namespace SJ
{
	bool FileExtractor::s_extracted = false;

	void FileExtractor::extractFiles()
	{
		std::thread thread(fileExtractorThread);
		thread.detach();//Detaches the thread so that it can run the extractor separately no rejoining
		//No rejoining because the file extractor class is a static which means it's going to be there for the lifetime of the program
	}

	void FileExtractor::fileExtractorThread()
	{
		s_extracted = false;
		for (const auto& entry : std::filesystem::directory_iterator(m_inputFolder))
		{
			std::wstring fileName = entry.path().filename().wstring(); //Get the filename that has the extension
			std::wstring folderName = entry.path().filename().wstring(); //Get the filename but without the extension to be used to create a folder
			folderName = folderName.substr(0, folderName.find_last_of('.')); //Remove the extension from the filename
			try
			{
				if (entry.path().filename().extension() == ".osz")
				{
					std::filesystem::current_path(m_inputFolder);//Set the path to the input folder
					std::wstring inputPath = std::filesystem::current_path();
					std::filesystem::current_path("../Songs");
					std::wstring songPath = std::filesystem::current_path();//Set the path to the songs folder
					if(!std::filesystem::exists(folderName))
					{
						std::filesystem::create_directory("x");//Create a temp folder so it's less likely to cause an error when extracting the file due to having a too long string
						songPath += '/'; songPath += L"x";//Append '/' and temp
						inputPath += '/'; inputPath += fileName;//Append '/' and folder name
						m_extractor.extract(inputPath, songPath);//Extract the file from the input folder and put into the songs folder
						std::filesystem::rename("x", folderName);//Rename the temp folder to the song name to prevent clashing with duplicate names
						std::filesystem::current_path("../Input");
						std::filesystem::remove(fileName);
						std::filesystem::current_path(m_origin);
					}
					else
					{
						std::filesystem::current_path("../Input");
						std::filesystem::remove(fileName);
					}
					#ifdef DEBUG
					std::wcout << "Extracted " << folderName << std::endl;
					#endif 
				}
			}
			catch (const BitException& e)
			{
				std::cout << e.what() << std::endl;
				std::filesystem::remove(folderName);
			}
			std::filesystem::current_path(m_origin);
		}
		s_extracted = true;
		#ifdef DEBUG
		std::cout << "Extraction complete" << std::endl;
		#endif
	}
}
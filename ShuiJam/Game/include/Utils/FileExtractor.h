/*\file FileExtractor.h*/
#pragma once
#include <bitextractor.hpp>
#include <bitexception.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <thread>

namespace SJ
{
	using namespace bit7z;
	/*\class FileExtractor*/
	class FileExtractor
	{
	private:
		const wchar_t* m_songsFolder = L"../SJAssets/Songs/";//!< Location of the songs folder
		const wchar_t* m_inputFolder = L"../SJAssets/Input/";//!< Location of the inputs folder
		std::filesystem::path m_origin;
		Bit7zLibrary m_lib{ L"../SJAssets/7z.dll" };//!< Location of the 7zip dll file
		BitExtractor m_extractor{ m_lib,BitFormat::Zip };//!< Extraction method

		static FileExtractor* s_extractor;//!< Singleton extractor class
		FileExtractor();
		~FileExtractor() {};

		void fileExtractorThread();
	public:
		FileExtractor(FileExtractor& other) = delete; //!< Prevents cloning another FileExtractor
		void operator=(const FileExtractor&) = delete; //!< Prevents setting another FileExtractor
		static FileExtractor* get();//!< Singleton getter
		void extractFiles(); //!< Extract 7zip file
	};
}

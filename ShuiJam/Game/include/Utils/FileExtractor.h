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
	/*\class FileExtractor
	\brief extracts .osz files into the songs folder*/
	class FileExtractor
	{
	private:
		const wchar_t* m_songsFolder = L"../ShuiJamGame/Songs/";//!< Location of the songs folder
		const wchar_t* m_inputFolder = L"../ShuiJamGame/Input/";//!< Location of the inputs folder
		std::filesystem::path m_origin;
		Bit7zLibrary m_lib{ L"../ShuiJamGame/7z.dll" };//!< Location of the 7zip dll file
		BitExtractor m_extractor{ m_lib,BitFormat::Zip };//!< Extraction method

		bool m_extracted;//!<For testing purposes to see if the file has been extracted successfully

		static FileExtractor* s_extractor;//!< Singleton extractor class
		FileExtractor();
		~FileExtractor() {};

		void fileExtractorThread();
	public:
		FileExtractor(FileExtractor& other) = delete; //!< Prevents cloning another FileExtractor
		void operator=(const FileExtractor&) = delete; //!< Prevents setting another FileExtractor
		static FileExtractor* get();//!< Singleton getter
		void extractFiles(); //!< Extract 7zip file
		inline bool isExtracted() { return m_extracted; }//!<Returns true if there was a file extracted
	};
}

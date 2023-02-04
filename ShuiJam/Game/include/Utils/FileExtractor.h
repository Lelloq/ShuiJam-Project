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
		const std::wstring m_songsFolder = L"../ShuiJamGame/Songs/";//!< Location of the songs folder
		const std::wstring m_inputFolder = L"../ShuiJamGame/Input/";//!< Location of the inputs folder
		const std::filesystem::path m_origin = std::filesystem::current_path();
		Bit7zLibrary m_lib{ L"../ShuiJamGame/7z.dll" };//!< Location of the 7zip dll file
		BitExtractor m_extractor{ m_lib,BitFormat::Zip };//!< Extraction method

		bool m_extracted = false;//!<If the file has been extracted successfully

		static std::unique_ptr<FileExtractor> s_extractor;//!< Singleton extractor class

		void fileExtractorThread();
	public:
		static std::unique_ptr<FileExtractor> get();//!< Singleton getter
		void extractFiles(); //!< Extract 7zip file
		inline bool isExtracted() { return m_extracted; }//!<Returns true if there was a file extracted
	};
}

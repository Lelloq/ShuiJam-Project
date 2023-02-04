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
		inline static const std::wstring m_songsFolder = L"../ShuiJamGame/Songs/";//!< Location of the songs folder
		inline static const std::wstring m_inputFolder = L"../ShuiJamGame/Input/";//!< Location of the inputs folder
		inline static const std::filesystem::path m_origin = std::filesystem::current_path();
		inline static Bit7zLibrary m_lib{ L"../ShuiJamGame/7z.dll" };//!< Location of the 7zip dll file
		inline static BitExtractor m_extractor{ m_lib,BitFormat::Zip };//!< Extraction method

		static bool s_extracted;//!<If the file has been extracted successfully
		static void fileExtractorThread();
	public:
		static void extractFiles(); //!< Extract 7zip file
		inline static bool isExtracted() { return s_extracted; }//!<Returns true if there was a file extracted
	};
}

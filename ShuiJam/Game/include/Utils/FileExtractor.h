/*\file FileExtractor.h*/
#pragma once
#include <bitextractor.hpp>
#include <bitexception.hpp>
#include <iostream>
#include <filesystem>

namespace SJ
{
	using namespace bit7z;
	/*\class FileExtractor*/
	class FileExtractor
	{
	private:
		Bit7zLibrary m_lib{ L"7z.dll" };
		BitExtractor m_extractor{ m_lib,BitFormat::Zip };
		const wchar_t* m_songsFolder = L"../SJAssets/Songs/";
		const wchar_t* m_inputFolder = L"../SJAssets/Input/";

		static FileExtractor* s_extractor;
		FileExtractor();
		~FileExtractor();
	public:
		static FileExtractor* get();
	};
}

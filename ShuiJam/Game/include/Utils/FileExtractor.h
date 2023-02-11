/*\file FileExtractor.h*/
#pragma once
#include <bitextractor.hpp>
#include <bitexception.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <future>

namespace SJ
{
	template<typename T>
	static bool isFutureReady(const std::future<T>& future) {
		return (future.wait_for(std::chrono::duration<float>(0.f)) == std::future_status::ready);
	}

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

		static bool fileExtractorThread();
	public:
		static bool extractFiles(); //!< Extract 7zip file
	};
}

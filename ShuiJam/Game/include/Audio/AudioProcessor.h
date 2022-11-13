/*\file AudioProcessor.h*/
#pragma once
#include <dr_wav.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <minimp3.h>
#include <minimp3_ex.h>
#include <filesystem>
#include <vector>
#include <iostream>

namespace SJ
{
	/*\struct WaveData*/
	struct WavData
	{
		std::vector<uint16_t> pcmData; //!< The PCM data of the audio file
		unsigned int channels = 0; //!< Channels for the wav data mono/stereo
		unsigned int sampleRate = 0; //!< Sample rate of the audio file
		drwav_uint64 totalPCMFrameCount = 0; //!< Total samples of the audio file
	};

	/*\struct MP3Data*/
	struct MP3Data
	{
		mp3d_sample_t* pcmData; //!< MP3 sample pointer, holds the PCM data
		unsigned int channels; //!< Channels for MP3 data mono/stereo
		unsigned int sampleRate; //!< Sample rate of the audio file
		size_t size;//!< Size of the buffer
	};

	/*\struct OggData*/
	struct OggData
	{
		uint8_t* pcmData;//!< Ogg unsigned char pointer, holds buffer data for the audio
		unsigned int channels;//!< Ogg channels
		long sampleRate;//!< Ogg sample rate
		uint64_t samples;//!< Ogg samples
		unsigned int size;//!< Ogg buffer size
	};

	class AudioProcessor
	{
	public:
		static WavData ProcessWavData(std::filesystem::path& filepath);//!< Decodes wav data
		static MP3Data ProcessMP3Data(std::filesystem::path& filepath);//!< Decodes mp3 data 
		static OggData ProcessOggData(std::filesystem::path& filepath);//!< Decodes ogg data
	};
}
/*\file AudioProcessor.h*/
#pragma once
#include <dr_wav.h>
#include <vorbis/codec.h>
#include <minimp3.h>
#include <minimp3_ex.h>
#include <string>
#include <vector>
#include <iostream>

/*\struct WaveData*/
struct WavData
{
	unsigned int channels = 0; //!< Channels for the wav data mono/stereo
	unsigned int sampleRate = 0; //!< Sample rate of the audio file
	drwav_uint64 totalPCMFrameCount = 0; //!< Total samples of the audio file
	std::vector<uint16_t> pcmData; //!< The PCM data of the audio file
};

/*\struct MP3Data*/
struct MP3Data
{
	mp3d_sample_t* buffer; //!< MP3 buffer pointer, holds the PCM data
	unsigned int channels; //!< Channels for MP3 data mono/stereo
	unsigned int sampleRate; //!< Sample rate of the audio file
	size_t size;//!< Size of the buffer
};

/*\struct OggData*/
struct OggData
{
	
};

class AudioProcessor
{
public:
	static WavData ProcessWavData(std::string& filepath);//!< Decodes wav data
	static MP3Data ProcessMP3Data(std::string& filepath);//!< Decodes mp3 data 
	static OggData ProcessOggData(std::string& filepath);//!< Decodes ogg data
};
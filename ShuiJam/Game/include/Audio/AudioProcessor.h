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
	unsigned int channels;
	unsigned int frame_bytes;
	unsigned int hz;
	unsigned int layer;
	unsigned int bitrate_kbps;
};

/*\struct OggData*/
struct OggData
{

};

class AudioProcessor
{
public:
	static WavData ProcessWavData(std::string& filepath);
	static MP3Data ProcessMP3Data(std::string& filepath);
	static OggData ProcessOggData(std::string& filepath);
};
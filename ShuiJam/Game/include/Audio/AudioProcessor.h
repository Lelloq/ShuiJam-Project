/*\file AudioProcessor.h*/
#pragma once
#include <dr_wav.h>
#include <vorbis/codec.h>
#include <minimp3.h>
#include <minimp3_ex.h>
#include <string>

/*\struct WaveData*/
struct WavData
{
	unsigned int channels;
	unsigned int sampleRate;
	drwav_int64 totalPCMFrameCount;
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
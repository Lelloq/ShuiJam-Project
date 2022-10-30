/*\file AudioProcessor.h*/
#pragma once
#include <dr_wav.h>
#include <vorbis/codec.h>
#include <minimp3.h>
#include <minimp3_ex.h>
#include <string>
#include <vector>

/*\struct WaveData*/
struct WavData
{
	drwav_uint32 channels; //!< Channels for the wav data mono/stereo
	drwav_uint32 sampleRate; //!< Sample rate of the audio file
	drwav_uint64 totalPCMFrameCount; //!< Total samples of the audio file
	std::vector<uint32_t> pcmData; //!< The PCM data of the audio file
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
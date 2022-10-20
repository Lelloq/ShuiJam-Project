#pragma once
#include "dr_wav.h"
#include "minimp3.h"
#include "minimp3_ex.h"

struct WavData
{
	unsigned int channels;
	unsigned int sampleRate;
	drwav_int64 totalPCMFrameCount;
};

struct Mp3Data
{
	
};

class AudioProcessor
{

};
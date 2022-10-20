#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

class AudioDevice
{
private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

	AudioDevice();
	~AudioDevice();
public:
	static AudioDevice* get();
};
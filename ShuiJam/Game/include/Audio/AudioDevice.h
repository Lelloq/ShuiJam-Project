#pragma once
#include "AL/alc.h"

class AudioDevice
{
private:
	static AudioDevice* s_audioDevice;
	ALCdevice* p_Device;
	ALCcontext* p_Context;

	AudioDevice();
public:
	static AudioDevice* getAudioDevice()
	{
		if(!s_audioDevice)
		{
			s_audioDevice = new AudioDevice;
			return s_audioDevice;
		}
	};

	void InitialiseDevice();
};
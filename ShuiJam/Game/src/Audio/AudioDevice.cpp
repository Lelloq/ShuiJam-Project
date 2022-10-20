#include "pch.h"
#include "Audio/AudioDevice.h"
#include <stdio.h>

AudioDevice* AudioDevice::get()
{
	static AudioDevice* snd_device = new AudioDevice();
	return snd_device;
}

AudioDevice::AudioDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context

	alcMakeContextCurrent(p_ALCContext);   // make context current
}

AudioDevice::~AudioDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}


#include "Audio/AudioDevice.h"

void AudioDevice::InitialiseDevice()
{
	p_Device = alcOpenDevice(nullptr);
	p_Context = alcCreateContext(p_Device, nullptr);
}
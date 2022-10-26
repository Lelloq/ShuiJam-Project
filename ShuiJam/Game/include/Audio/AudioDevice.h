/*\file AudioDevice.h*/
#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <mutex>

class AudioDevice
{
private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

	static AudioDevice* s_device;
	static std::mutex m;

	AudioDevice();
	~AudioDevice();
public:
	AudioDevice(AudioDevice& other) = delete; //!< Prevents cloning another AudioDevice
	void operator=(const AudioDevice&) = delete; //!< Prevents setting another AudioDevice
	static AudioDevice* get();//!< Gets instance of AudioDevice
};
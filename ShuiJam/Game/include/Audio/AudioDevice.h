/*\file AudioDevice.h*/
#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <mutex>

namespace SJ
{
	/*\class AudioDevice
	Static class in a singleton*/
	class AudioDevice
	{
	private:
		ALCdevice* m_ALCDevice; //!< ALCdevice pointer
		ALCcontext* m_ALCContext; //!< ALCcontext pointer

		static AudioDevice* s_device; //!< Audio device singleton instance
		static std::mutex m; //!< Static mutex guard member

		AudioDevice();
		~AudioDevice();
	public:
		AudioDevice(AudioDevice& other) = delete; //!< Prevents cloning another AudioDevice
		void operator=(const AudioDevice&) = delete; //!< Prevents setting another AudioDevice
		static AudioDevice* get();//!< Gets instance of AudioDevice
		void setGain(float gain);//!< Set audio volume
	};
}
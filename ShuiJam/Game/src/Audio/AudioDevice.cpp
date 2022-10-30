/*\file AudioDevice.cpp*/
#include "pch.h"
#include "Audio/AudioDevice.h"

//Defining the members
AudioDevice* AudioDevice::s_device{ nullptr };//Assigns s_device to nullptr
std::mutex AudioDevice::m; //Assigns m for the mutex to be locked during the get instance

//Using mutex to create a thread safe singleton for the audio device
AudioDevice* AudioDevice::get()
{
	std::lock_guard<std::mutex> lock(m);
	if(s_device == nullptr)
	{
		s_device = new AudioDevice();
	}
	return s_device;
}

//Create the OpenAL sound device in the constructor
AudioDevice::AudioDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context

	alcMakeContextCurrent(p_ALCContext);   // make context current
}

//Closes the OpenAL sound device in the destructor
AudioDevice::~AudioDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}


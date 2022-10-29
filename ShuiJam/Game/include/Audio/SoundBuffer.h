#pragma once
#include <AL/al.h>
#include <map>
#include <mutex>
#include <string>

class SoundBuffer
{
private:
	SoundBuffer();
	~SoundBuffer();

	static SoundBuffer* s_Buffer; //!< Static sound buffer singleton
	static std::mutex m; //!< mutex lock guard for sound buffer

	std::map<std::string, ALuint> p_SFXBuffers; //!< Key value map of sfx buffers
public:
	SoundBuffer(SoundBuffer& other) = delete; //!< Prevents cloning another SoundBuffer
	void operator=(const SoundBuffer&) = delete; //!< Prevents setting another SoundBuffer
	static SoundBuffer* getSoundBuffer(); //!< gets instance of sound buffers

	ALuint addSFX(std::string name, std::string filename); //!< adds sound effect
	void removeSFX(const std::string name); //!< removes sound effect
};
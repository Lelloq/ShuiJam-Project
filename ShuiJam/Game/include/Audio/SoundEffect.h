/*\file SoundEffect.h*/
#pragma once
#include <AL/al.h>
#include <vector>
#include <mutex>
#include <filesystem>

/*\class SoundEffect*/
class SoundEffect
{
private:
	SoundEffect();
	~SoundEffect();

	static SoundEffect* s_Buffer; //!< Static sound buffer singleton
	static std::mutex m; //!< mutex lock guard for sound buffer

	std::vector<ALuint> p_SFXBuffers; //!< Key value map of sfx buffers
public:
	SoundEffect(SoundEffect& other) = delete; //!< Prevents cloning another SoundBuffer
	void operator=(const SoundEffect&) = delete; //!< Prevents setting another SoundBuffer
	static SoundEffect* get(); //!< gets instance of sound buffers

	ALuint addSFX(std::filesystem::path filepath); //!< adds sound effect
	void removeSFX(const ALuint& buffer); //!< removes sound effect
};

/*\class SFXSource*/
class SFXSource
{
private:
	ALuint m_source;
	ALuint m_buffer;

public:
	SFXSource();
	~SFXSource();

	void Play(const ALuint buffer);
};
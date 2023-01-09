/*\file SoundEffect.h*/
#pragma once
#include <AL/al.h>
#include <vector>
#include <mutex>
#include <filesystem>
namespace SJ
{
	/*\class SoundEffect
	Static class that uses OpenAL and handles sound effects, DO NOT put music in the buffers*/
	class SoundEffect
	{
	private:
		SoundEffect();
		~SoundEffect();

		static SoundEffect* s_Buffer; //!< Static sound buffer singleton
		static std::mutex m; //!< mutex lock guard for sound buffer

		std::vector<ALuint> m_SFXBuffers; //!< Key value map of sfx buffers
	public:
		SoundEffect(SoundEffect& other) = delete; //!< Prevents cloning another SoundBuffer
		void operator=(const SoundEffect&) = delete; //!< Prevents setting another SoundBuffer
		static SoundEffect* get(); //!< gets instance of sound buffers

		ALuint addSFX(std::filesystem::path filepath); //!< adds sound effect
		void removeSFX(const ALuint& buffer); //!< removes sound effect
	};

	/*\class SFXSource
	\Sound source for the sound effects - if you want to play multiple you need to make multiple sound sources*/
	class SFXSource
	{
	private:
		ALuint m_source;
		ALuint m_buffer = 0;

	public:
		SFXSource();
		~SFXSource();

		void Play(const ALuint buffer);
	};
}
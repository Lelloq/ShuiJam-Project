/*\file SoundBuffer.cpp*/
#include "Audio/SoundEffect.h"
#include "Audio/AudioProcessor.h"
namespace SJ
{
	SoundEffect* SoundEffect::s_Buffer = nullptr;
	std::mutex SoundEffect::m;

	SoundEffect::SoundEffect()
	{
		m_SFXBuffers.clear();
	}

	SoundEffect::~SoundEffect()
	{
		alDeleteBuffers(m_SFXBuffers.size(), m_SFXBuffers.data());
		m_SFXBuffers.clear();
	}

	SoundEffect* SoundEffect::get()
	{
		std::lock_guard<std::mutex> lock(m);
		if(s_Buffer == nullptr)
		{
			s_Buffer = new SoundEffect();
		}
		return s_Buffer;
	}

	ALuint SoundEffect::addSFX(std::filesystem::path filepath)
	{
		//Initially taken from "Code, Tech, and Tutorials" modified for it to work using other file types
		ALenum format;
		ALuint buffer;

		//Each of the conditions does the same thing but for different files
		//Get the audio format (mono/stereo)
		//Generate albuffer and fill the buffer with data
		//Push the buffer into the vector of sound effects
		if (filepath.extension() == ".wav")
		{
			WavData data = AudioProcessor::ProcessWavData(filepath);

			format = AL_NONE;
			if (data.channels == 1) format = AL_FORMAT_MONO16;
			else format = AL_FORMAT_STEREO16;

			alGenBuffers(1, &buffer);
			alBufferData(buffer, format, data.buffer, data.size, data.sampleRate);

			m_SFXBuffers.push_back(buffer);
			return buffer;
		}
		else if(filepath.extension() == ".mp3")
		{
			MP3Data data = AudioProcessor::ProcessMP3Data(filepath);

			format = AL_NONE;
			if (data.channels == 1) format = AL_FORMAT_MONO16;
			else format = AL_FORMAT_STEREO16;

			alGenBuffers(1, &buffer);
			alBufferData(buffer, format, data.buffer, data.size, data.sampleRate);

			m_SFXBuffers.push_back(buffer);
			return buffer;
		}
		else if(filepath.extension() == ".ogg")
		{
			OggData data = AudioProcessor::ProcessOggData(filepath);

			format = AL_NONE;
			if (data.channels == 1) format = AL_FORMAT_MONO16;
			else format = AL_FORMAT_STEREO16;

			alGenBuffers(1, &buffer);
			alBufferData(buffer, format, data.buffer, data.size, data.sampleRate);

			m_SFXBuffers.push_back(buffer);
			return buffer;
		}
		std::cout << "ERROR: Attempted to process an invalid song file" << std::endl;
		return ALuint();
	}

	void SoundEffect::removeSFX(const ALuint& buffer)
	{
		for(auto it = m_SFXBuffers.begin(); it != m_SFXBuffers.end(); ++it)
		{
			if(*it == buffer)
			{
				alSourceStop(*it);
				alDeleteSources(1, &*it);
				m_SFXBuffers.erase(it);
			}
		}
	}

	SFXSource::SFXSource()
	{
		alGenSources(1, &m_source);
		alSourcef(m_source, AL_PITCH, 1);
		alSourcef(m_source, AL_GAIN, 1);
		alSourcei(m_source, AL_LOOPING, false);
		alSourcei(m_source, AL_BUFFER, m_buffer);
	}

	SFXSource::~SFXSource()
	{
		alDeleteSources(1, &m_source);
	}

	void SFXSource::Play(const ALuint buffer)
	{
		if(buffer != m_buffer)
		{
			m_buffer = buffer;
			alSourcei(m_source, AL_BUFFER, (ALint)m_buffer);
		}
		alSourcePlay(m_source);
	}
}

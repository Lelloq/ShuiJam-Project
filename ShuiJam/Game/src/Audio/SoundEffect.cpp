/*\file SoundBuffer.cpp*/
#include "Audio/SoundEffect.h"
#include "Audio/AudioProcessor.h"

SoundEffect* SoundEffect::s_Buffer{nullptr};
std::mutex SoundEffect::m;

SoundEffect::SoundEffect()
{
	p_SFXBuffers.clear();
}

SoundEffect::~SoundEffect()
{
	alDeleteBuffers(p_SFXBuffers.size(), p_SFXBuffers.data());
	p_SFXBuffers.clear();
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

ALuint SoundEffect::addSFX(std::string filename)
{
	ALenum format;
	ALuint buffer;

	if (filename.find(".wav"))
	{
		WavData data = AudioProcessor::ProcessWavData(filename);

		format = AL_NONE;
		if (data.channels == 1) format = AL_FORMAT_MONO16;
		else format = AL_FORMAT_STEREO16;

		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, data.pcmData.data(), data.pcmData.size(), data.sampleRate);

		p_SFXBuffers.push_back(buffer);
		return buffer;
	}
	else if(filename.find(".mp3"))
	{
		return buffer;
	}
	else if(filename.find(".ogg"))
	{
		return buffer;
	}
	return ALuint();
}

void SoundEffect::removeSFX(const ALuint& buffer)
{
	for(auto it = p_SFXBuffers.begin(); it != p_SFXBuffers.end(); ++it)
	{
		if(*it == buffer)
		{
			alDeleteBuffers(1, &*it);
			p_SFXBuffers.erase(it);
		}
	}
}

SFXSource::SFXSource()
{
	m_buffer = 0;
	alGenSources(1, &m_source);
	alSourcef(m_source, AL_PITCH, 1);
	alSourcef(m_source, AL_GAIN, 1);
	alSource3f(m_source, AL_POSITION, 0, 0, 0);
	alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
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
		alSourcei(m_source, AL_BUFFER, (ALint)buffer);
	}
	alSourcePlay(m_source);
}
/*\file SoundBuffer.cpp*/
#include "pch.h"
#include "Audio/SoundBuffer.h"
#include "Audio/AudioProcessor.h"

SoundBuffer* SoundBuffer::s_Buffer{nullptr};
std::mutex SoundBuffer::m;

SoundBuffer::SoundBuffer()
{
	p_SFXBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(p_SFXBuffers.size(), p_SFXBuffers.data());
	p_SFXBuffers.clear();
}

SoundBuffer* SoundBuffer::getSoundBuffer()
{
	std::lock_guard<std::mutex> lock(m);
	if(s_Buffer == nullptr)
	{
		s_Buffer = new SoundBuffer();
	}
	return s_Buffer;
}

ALuint SoundBuffer::addSFX(std::string name,std::string filename)
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

void SoundBuffer::removeSFX(const ALuint& buffer)
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

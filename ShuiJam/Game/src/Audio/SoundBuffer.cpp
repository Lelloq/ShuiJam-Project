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
	for(auto it = p_SFXBuffers.begin(); it != p_SFXBuffers.end(); ++it)
	{
		alDeleteBuffers(1, &it->second);
	}
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
	if (filename.find(".mp3"))
	{
		MP3Data data = AudioProcessor::ProcessMP3Data(filename);
	}
	return ALuint();
}

void SoundBuffer::removeSFX(const std::string name)
{
	alDeleteBuffers(1, &p_SFXBuffers[name]);
	p_SFXBuffers.erase(name);
}

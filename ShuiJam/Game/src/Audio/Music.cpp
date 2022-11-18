/*\file Music.cpp*/
#include "Audio/Music.h"

namespace SJ
{
	Music::Music(const char* fileName)
	{
		alGenSources(1, &m_source);
		alGenBuffers(NUM_BUFFERS, m_buffers);


	}
	Music::~Music()
	{
	}
	void Music::Play()
	{
	}
	void Music::Stop()
	{
	}
}
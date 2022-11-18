/*\file Music.h*/
#pragma once
#include "AudioProcessor.h"
#include <AL/al.h>

namespace SJ
{
	/*\class Music
	Class that plays music through OpenAL*/
	class Music
	{
	private:
		ALuint m_source;//!<OpenAL sound source
		static const std::size_t NUM_BUFFERS = 4;//!<Number of buffers inside a buffer
		static const ALsizei BUFFER_SIZE = 65536;//!<Size of the buffer
		ALuint m_buffers[NUM_BUFFERS];//!<Number of buffers
		ALenum m_format;//!<OpenAL audio format
		short* m_membuf;//!<Current buffer stored in memory
		WavData wavData;//!<Empty WavData struct if the file reads a wav file
		MP3Data Mp3Data;//!<Empty Mp3Data struct if the file reads a mp3 file
		OggData OggData;//!<Empty OggData struct if the file reads an ogg file

		Music() = delete;//!<Deletes the object if nothing is given
	public:
		Music(const char* filePath);//!<Loads music with the assigned file path
		~Music();//!<Destructor
		void Play();//!<Plays the music
		void Stop();//!<Stops the music
	};
}
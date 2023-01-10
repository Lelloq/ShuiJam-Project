/*\file Music.h*/
#pragma once
#include "AudioProcessor.h"
#include <dr_mp3.h>
#include <AL/al.h>
#include <chrono>
#include <thread>

namespace SJ
{
	static const std::size_t NUM_BUFFERS = 4;//!<Number of buffers inside a buffer
	static const ALsizei BUFFER_SIZE = 8192;//!<Size of the buffer

	struct MP3StreamData
	{
		drmp3 mp3;
		int16_t* buffer;
	};

	struct WavStreamData
	{
		drwav wav;
		int16_t* buffer;
	};

	struct OggStreamData
	{
		OggVorbis_File vfile;
		vorbis_info* info;
	};

	/*\class Music
	Class that plays music through OpenAL*/
	class Music
	{
	private:
		ALuint m_source;//!<OpenAL sound source
		ALuint m_buffers[NUM_BUFFERS];//!<Number of buffers
		ALenum m_format;//!<OpenAL audio format
		std::unique_ptr<MP3StreamData> mStream;//!<Empty Mp3Data struct if the file reads a mp3 file
		std::unique_ptr<OggStreamData> oStream;//!<Empty OggData struct if the file reads an ogg file
		std::unique_ptr<WavStreamData> wStream;//!<Empty WavData struct if the file reads a wav file

		double m_timepos = 0.f;
		bool m_atEnd = false;
		std::size_t duration = 0;
		std::string m_extension;

		void timerThread();//!<Function that accumulates time, done on a separate thread
		void startTimer();//!<Creates a thread that starts the timer
	public:
		Music(std::filesystem::path filePath);//!<Loads music with the assigned file path
		~Music();//!<Destructor
		void Play();//!<Plays the music
		void Stop();//!<Stops the music
		void Update();//!<Updates the buffer stream
		inline double getTimePosition() { return m_timepos; }//!<Get time position in milliseconds
	};
}
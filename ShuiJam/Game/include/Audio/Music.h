/*\file Music.h*/
#pragma once
#include "AudioProcessor.h"
#include <dr_mp3.h>
#include <AL/al.h>
#include <chrono>
#include <thread>
#include <fstream>

namespace SJ
{
	static const std::size_t NUM_BUFFERS = 8;//!<Number of buffers inside a buffer
	static const ALsizei BUFFER_SIZE = 8192;//!<Size of the buffer

	/*\struct MP3StreamData
	\brief Struct that holds data required to stream MP3 audio*/
	struct MP3StreamData
	{
		drmp3 mp3;
		int16_t* buffer;
	};

	/*\struct WavStreamData
	\brief Struct that holds data required to stream wav audio*/
	struct WavStreamData
	{
		drwav wav;
		int16_t* buffer;
	};

	/*\struct OggStreamData
	\brief Struct that holds data required to stream ogg audio*/
	struct OggStreamData
	{
		static FILE* file;
		OggVorbis_File vfile;
		vorbis_info* info;
		int16_t* buffer;
		int section;
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

		int m_timepos = 0;//!<Time position of the song in milliseconds
		bool m_atEnd = false;//!<Is the song at the end
		std::string m_extension;//!<Song file extension

		void timerThread();//!<Function that accumulates time, done on a separate thread
		void startTimer();//!<Creates a thread that starts the timer
	public:
		Music(std::filesystem::path filePath);//!<Loads music with the assigned file path
		~Music();//!<Destructor
		void Play();//!<Plays the music
		void Stop();//!<Stops the music
		void Update();//!<Updates the buffer stream
		inline int getTimePosition() { return m_timepos; }//!<Get time position in milliseconds
	};
}
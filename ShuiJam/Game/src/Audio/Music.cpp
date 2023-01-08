/*\file Music.cpp*/
#include "Audio/Music.h"

namespace SJ
{
	Music::Music(std::filesystem::path filePath) : mStream(new MP3StreamData())
	{
		alGenSources(1, &m_source);
		alGenBuffers(NUM_BUFFERS, m_buffers);

		std::size_t frame_size;

		if(filePath.extension() == ".mp3")
		{
			m_extension = ".mp3";
			if (!drmp3_init_file(&mStream->mp3, filePath.string().c_str(), NULL)) std::cout << "Failed to load song file: " << filePath.string() << std::endl;
			else
			{
				if (mStream->mp3.channels == 1) m_format = AL_FORMAT_MONO16;
				else if (mStream->mp3.channels == 2) m_format = AL_FORMAT_STEREO16;
				
				frame_size = (size_t)(BUFFER_SIZE * mStream->mp3.channels) * 2;
				mStream->buffer = new int16_t[frame_size];
			}
		}
		
	}
	Music::~Music()
	{
		alDeleteSources(1, &m_source);

		if (mStream != nullptr) delete mStream;

		alDeleteBuffers(NUM_BUFFERS, m_buffers);
	}
	void Music::Play()
	{
		//Original taken from "Code, Tech, and Tutorials" modified for use to make it work on other file types
		ALsizei i;

		alSourceRewind(m_source);//Rewind the audio source back to 0 seconds
		alSourcei(m_source, AL_BUFFER, 0);//Clear buffer so it can be filled layer
		if (m_extension == ".mp3")
		{
			for (i = 0; i < NUM_BUFFERS; i++)
			{
				drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream->mp3, BUFFER_SIZE, mStream->buffer);//Frame data
				if (sample < 1) break;
				sample *= mStream->mp3.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
				alBufferData(m_buffers[i], m_format, mStream->buffer, (ALsizei)sample, mStream->mp3.sampleRate);
			}

			if(alGetError() != AL_NO_ERROR)
			{
				std::cout << "Error buffering for playback" << std::endl;
			}

			alSourceQueueBuffers(m_source, i, m_buffers);
			alSourcePlay(m_source);
		}
	}

	void Music::Stop()
	{

	}

	void Music::Update()
	{
		//Original taken from "Code, Tech, and Tutorials" modified for use to make it work on other file types
		ALint processed, state;

		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
		if(alGetError() != AL_NO_ERROR)
		{
			throw("Error checking music source state");
		}

		while(processed > 0)
		{
			ALuint bufid;
			
			alSourceUnqueueBuffers(m_source, 1, &bufid);
			processed--;

			if(m_extension == ".mp3")
			{
				drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream->mp3, BUFFER_SIZE, mStream->buffer);//Frame data
				if(sample > 0)
				{
					sample *= mStream->mp3.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(bufid, m_format, mStream->buffer, (ALsizei)sample, mStream->mp3.sampleRate);
					alSourceQueueBuffers(m_source, 1, &bufid);
				}
			}
		}

		if(state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) return;

			alSourcePlay(m_source);
		}
	}
}
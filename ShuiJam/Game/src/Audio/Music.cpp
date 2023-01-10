/*\file Music.cpp*/
#include "Audio/Music.h"

namespace SJ
{
	Music::Music(std::filesystem::path filePath)
	{
		alGenSources(1, &m_source);
		alGenBuffers(NUM_BUFFERS, m_buffers);

		std::size_t frame_size;

		//Open the file using drmp3 if it detects a mp3 file
		if(filePath.extension() == ".mp3")
		{
			mStream.reset(new MP3StreamData);
			m_extension = ".mp3";
			if (!drmp3_init_file(&mStream->mp3, filePath.string().c_str(), NULL)) std::cout << "Failed to load song file: " << filePath.string() << std::endl;
			else
			{
				if (mStream->mp3.channels == 1) m_format = AL_FORMAT_MONO16;
				else if (mStream->mp3.channels == 2) m_format = AL_FORMAT_STEREO16;
				
				//Allocate the buffer which will be used to fill the queued buffers
				frame_size = (size_t)(BUFFER_SIZE * mStream->mp3.channels) * 2;
				mStream->buffer = new int16_t[frame_size];
			}
		}
		//Open the file using drwav if it detects a wav file
		else if(filePath.extension() == ".wav")
		{
			wStream.reset(new WavStreamData);
			m_extension = ".wav";
			if (!drwav_init_file(&wStream->wav, filePath.string().c_str(), NULL)) std::cout << "Failed to load song file: " << filePath.string() << std::endl;
			else
			{
				if (wStream->wav.channels == 1) m_format = AL_FORMAT_MONO16;
				else if (wStream->wav.channels == 2) m_format = AL_FORMAT_STEREO16;

				//Allocate the buffer which will be used to fill the queued buffers
				frame_size = (size_t)(BUFFER_SIZE * wStream->wav.channels) * 2;
				wStream->buffer = new int16_t[frame_size];
			}
		}
	}
	Music::~Music()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
	}
	void Music::Play()
	{
		//Original taken from "Code, Tech, and Tutorials" modified for use to make it work on other file types
		ALsizei i, state;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);//Prevent playing if its already playing
		if(state != AL_PLAYING)
		{
			alSourceRewind(m_source);//Rewind the audio source back to 0 seconds
			alSourcei(m_source, AL_BUFFER, 0);//Clear buffer so it can be filled layer
			if (m_extension == ".mp3")
			{
				for (i = 0; i < NUM_BUFFERS; i++)
				{
					drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream->mp3, BUFFER_SIZE, mStream->buffer);//Frame data
					if (sample < 1) break;
					sample *= mStream->mp3.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(m_buffers[i], m_format, mStream->buffer, (ALsizei)sample, mStream->mp3.sampleRate);//Fill al buffer data
				}
				if(alGetError() != AL_NO_ERROR)
				{
					std::cout << "Error buffering for playback" << std::endl;
				}

				alSourceQueueBuffers(m_source, i, m_buffers);
				alSourcePlay(m_source);
			}
			else if(m_extension == ".wav")
			{
				for (i = 0; i < NUM_BUFFERS; i++)
				{
					drwav_uint64 sample = drwav_read_pcm_frames_s16(&wStream->wav, BUFFER_SIZE, wStream->buffer);//Frame data
					if (sample < 1) break;
					sample *= wStream->wav.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(m_buffers[i], m_format, wStream->buffer, (ALsizei)sample, wStream->wav.sampleRate);//Fill al buffer data
				}

				if (alGetError() != AL_NO_ERROR)
				{
					std::cout << "Error buffering for playback" << std::endl;
				}

				alSourceQueueBuffers(m_source, i, m_buffers);
				alSourcePlay(m_source);
			}
			startTimer();
		}
	}

	void Music::Stop()
	{
		alSourceStop(m_source);
		if (mStream != NULL) mStream.release(); return;
		if (wStream != NULL) wStream.release(); return;
		if (oStream != NULL) oStream.release(); return;
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
					alBufferData(bufid, m_format, mStream->buffer, (ALsizei)sample, mStream->mp3.sampleRate);//Fill al buffer data
					alSourceQueueBuffers(m_source, 1, &bufid);//Add the buffer to the queue
				}
			}
			else if(m_extension == ".wav")
			{
				drwav_uint64 sample = drwav_read_pcm_frames_s16(&wStream->wav, BUFFER_SIZE, wStream->buffer);//Frame data
				if (sample > 0)
				{
					sample *= wStream->wav.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(bufid, m_format, wStream->buffer, (ALsizei)sample, wStream->wav.sampleRate);//Fill al buffer data
					alSourceQueueBuffers(m_source, 1, &bufid);//Add the buffer to the queue
				}
			}
		}

		//Keep playing the source to the end unless it has been paused or stopped
		if(state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) return;

			alSourcePlay(m_source);
		}
	}

	//Needs to be changed somewhat in the future to account for lag spikes
	void Music::timerThread()
	{
		using namespace std;
		m_atEnd = false;
		while(!mStream->mp3.atEnd)
		{
			auto start = chrono::steady_clock::now();
			this_thread::sleep_for(1ms);
			auto end = chrono::steady_clock::now();
			m_timepos += chrono::duration<double>(end - start).count();
			std::cout << m_timepos << std::endl;
		}
		m_atEnd = true;
		startTimer();
	}

	void Music::startTimer()
	{
		std::thread tThread(&Music::timerThread, this);
		if (!m_atEnd) tThread.detach();
		else tThread.join();
	}
}
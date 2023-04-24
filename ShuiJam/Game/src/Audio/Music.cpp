/*\file Music.cpp*/
#include "Audio/Music.h"
#include <future>

namespace SJ
{
	FILE* file;//Created a global file since creating it in the struct in the header file caused errors
	Music::Music(std::filesystem::path filePath)
	{
		alGenSources(1, &m_source);
		alGenBuffers(NUM_BUFFERS, m_buffers);

		std::size_t frame_size;

		//Open the file using drmp3 if it detects a mp3 file
		if(filePath.extension() == ".mp3")
		{
			mStream = MP3StreamData();
			m_extension = ".mp3";
			if (!drmp3_init_file(&mStream.mp3, filePath.string().c_str(), NULL)) std::cout << "Failed to load song file: " << filePath.string() << std::endl;
			else
			{
				if (mStream.mp3.channels == 1) m_format = AL_FORMAT_MONO16;
				else if (mStream.mp3.channels == 2) m_format = AL_FORMAT_STEREO16;
				
				//Allocate the buffer which will be used to fill the queued buffers
				frame_size = (size_t)(BUFFER_SIZE * mStream.mp3.channels) * 2;
				mStream.buffer = new int16_t[frame_size];
				m_sampleRate = mStream.mp3.sampleRate;
			}
		}
		//Open the file using drwav if it detects a wav file
		else if(filePath.extension() == ".wav")
		{
			wStream = WavStreamData();
			m_extension = ".wav";
			if (!drwav_init_file(&wStream.wav, filePath.string().c_str(), NULL)) std::cout << "Failed to load song file: " << filePath.string() << std::endl;
			else
			{
				if (wStream.wav.channels == 1) m_format = AL_FORMAT_MONO16;
				else if (wStream.wav.channels == 2) m_format = AL_FORMAT_STEREO16;

				//Allocate the buffer which will be used to fill the queued buffers
				frame_size = (size_t)(BUFFER_SIZE * wStream.wav.channels) * 2;
				wStream.buffer = new int16_t[frame_size];
				m_sampleRate = wStream.wav.sampleRate;
			}
		}
		//Open the file using vorbis if it detects an ogg file
		else if(filePath.extension() == ".ogg")
		{
			fopen_s(&file, filePath.string().c_str(), "rb");

			oStream = OggStreamData();
			m_extension = ".ogg";
			if (ov_open_callbacks(file, &oStream.vfile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) { std::cout << "Failed to load song file: " << filePath.string() << std::endl; }
			else
			{
				oStream.info = ov_info(&oStream.vfile, -1);
				if (oStream.info->channels == 1) m_format = AL_FORMAT_MONO16;
				else if (oStream.info->channels == 2) m_format = AL_FORMAT_STEREO16;

				frame_size = (size_t)(BUFFER_SIZE * oStream.info->channels) * 2;
				oStream.buffer = new int16_t[frame_size];
				m_sampleRate = oStream.info->rate;
			}
		}
	}
	Music::~Music()
	{
		m_atEnd = true;
		alDeleteSources(1, &m_source);
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		if(m_extension == ".mp3")
		{
			drmp3_free(mStream.buffer, nullptr);
		}
		else if(m_extension == ".wav")
		{
			drwav_free(wStream.buffer, nullptr);
		}
		else if(m_extension == ".ogg")
		{
			ov_clear(&oStream.vfile);
			fclose(file);
		}
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
					drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream.mp3, BUFFER_SIZE, mStream.buffer);//Frame data
					if (sample < 1) break;
					sample *= mStream.mp3.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(m_buffers[i], m_format, mStream.buffer, (ALsizei)sample, mStream.mp3.sampleRate);//Fill al buffer data
				}
				if(alGetError() != AL_NO_ERROR)
				{
					std::cout << "Error buffering for playback" << std::endl;
				}
				//Queue and play buffers
				alSourceQueueBuffers(m_source, i, m_buffers);
				alSourcePlay(m_source);
			}
			else if(m_extension == ".wav")
			{
				for (i = 0; i < NUM_BUFFERS; i++)
				{
					drwav_uint64 sample = drwav_read_pcm_frames_s16(&wStream.wav, BUFFER_SIZE, wStream.buffer);//Frame data
					if (sample < 1) break;
					sample *= wStream.wav.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(m_buffers[i], m_format, wStream.buffer, (ALsizei)sample, wStream.wav.sampleRate);//Fill al buffer data
				}

				if (alGetError() != AL_NO_ERROR)
				{
					std::cout << "Error buffering for playback" << std::endl;
				}
				
				alSourceQueueBuffers(m_source, i, m_buffers);
				alSourcePlay(m_source);
			}
			else if(m_extension == ".ogg")
			{
				//Read the bytes using ovread and fill each buffer with data
				for(i = 0; i < NUM_BUFFERS; i++)
				{
					long sample = ov_read(&oStream.vfile, (char*)oStream.buffer, BUFFER_SIZE, 0, 2, 1, &oStream.section);
					if (sample < 1) break;
					alBufferData(m_buffers[i], m_format, oStream.buffer, (ALsizei)sample, oStream.info->rate);
				}

				alSourceQueueBuffers(m_source, i, m_buffers);
				alSourcePlay(m_source);
			}
		}
	}

	void Music::Stop()
	{
		//Stop the music and close the file if its using vorbis
		alSourceStop(m_source);
		alSourcei(m_source, AL_BUFFER, NULL);
	}

	void Music::Update()
	{
		//Original taken from "Code, Tech, and Tutorials" modified for use to make it work on other file types
		ALint processed, state;

		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) return;

		alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
		if(alGetError() != AL_NO_ERROR)
		{
			throw("Error checking music source state");
		}

		while(processed > 0)
		{
			ALuint bufid;
			ALint sizeInBytes;
			ALint channels;
			ALint bits;

			alSourceUnqueueBuffers(m_source, 1, &bufid);

			alGetBufferi(bufid, AL_SIZE, &sizeInBytes);
			alGetBufferi(bufid, AL_CHANNELS, &channels);
			alGetBufferi(bufid, AL_BITS, &bits);
			m_samplesProcessed += sizeInBytes * 8 / (channels * bits);

			processed--;

			if(m_extension == ".mp3")
			{
				drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream.mp3, BUFFER_SIZE, mStream.buffer);//Frame data
				if(sample > 0)
				{
					sample *= mStream.mp3.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(bufid, m_format, mStream.buffer, (ALsizei)sample, mStream.mp3.sampleRate);//Fill al buffer data
					alSourceQueueBuffers(m_source, 1, &bufid);//Add the buffer to the queue
				}
			}
			else if(m_extension == ".wav")
			{
				drwav_uint64 sample = drwav_read_pcm_frames_s16(&wStream.wav, BUFFER_SIZE, wStream.buffer);//Frame data
				if (sample > 0)
				{
					sample *= wStream.wav.channels * 2;//Resize it to channels by size of a short (without this it will playback really fast)
					alBufferData(bufid, m_format, wStream.buffer, (ALsizei)sample, wStream.wav.sampleRate);//Fill al buffer data
					alSourceQueueBuffers(m_source, 1, &bufid);//Add the buffer to the queue
				}
			}
			else if(m_extension == ".ogg")
			{
				//Read the bytes up to 8192 bytes and then fill the buffer with data
				long sample = ov_read(&oStream.vfile, (char*)oStream.buffer, BUFFER_SIZE, 0, 2, 1, &oStream.section);
				if(sample > 0)
				{
					alBufferData(bufid, m_format, oStream.buffer, (ALsizei)sample, oStream.info->rate);
					alSourceQueueBuffers(m_source, 1, &bufid);
				}
			}
		}

		ALfloat offset;
		alGetSourcef(m_source, AL_SAMPLE_OFFSET, &offset);
		m_timepos = ((m_samplesProcessed + offset) / m_sampleRate) * 1000.f;

		//Keep playing the source to the end unless it has been paused or stopped
		if(state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) 
			{
				m_atEnd = true;
				Stop();
				return; 
			}

			alSourcePlay(m_source);
		}
	}
}
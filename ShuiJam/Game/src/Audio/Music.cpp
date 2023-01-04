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
				
				frame_size = (size_t)BUFFER_SIZE * mStream->mp3.channels * sizeof(short);
				mStream->buffer = static_cast<short*>(malloc(frame_size));
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
		alSourceRewind(m_source);
		alSourcei(m_source, AL_BUFFER, 0);
		if(m_extension == ".mp3")
		{
			for(int i = 0; i < NUM_BUFFERS; i++)
			{
				drmp3_uint64 sample = drmp3_read_pcm_frames_s16(&mStream->mp3,1,mStream->buffer);

				alBufferData(m_buffers[i], m_format, mStream->buffer, mStream->mp3.channels * sizeof(short), mStream->mp3.sampleRate);
			}
		}

		alSourcePlay(m_source);
	}

	void Music::Stop()
	{

	}

	void Music::Update()
	{
	}
}
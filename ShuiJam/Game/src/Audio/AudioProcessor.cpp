/*\file AudioProcessor.cpp*/
#include "Audio/AudioProcessor.h"
namespace SJ
{
	WavData AudioProcessor::ProcessWavData(std::filesystem::path& filepath)
	{
		WavData data = WavData(); //Empty struct
		drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(filepath.string().c_str(), &data.channels, &data.sampleRate, &data.totalPCMFrameCount, nullptr);
		if(pSampleData == NULL)
		{
			std::cout << "Failed to load wav file from path: " << filepath;
			drwav_free(pSampleData, nullptr);
			return WavData();
		}
		data.size = size_t(data.totalPCMFrameCount * data.channels) * 2;//Size of all the samples
		data.buffer = new uint16_t[data.size]; //Adjusts the size to fit the pcm data
		std::memcpy(data.buffer, pSampleData, data.size);//Copies the pcm data to the buffer
		drwav_free(pSampleData, nullptr);//Frees the data
		return data;
	}

	MP3Data AudioProcessor::ProcessMP3Data(std::filesystem::path& filepath)
	{
		MP3Data data = MP3Data(); //Empty struct
		mp3dec_t mp3d;//MP3 decoder
		mp3dec_file_info_t info;//MP3 info
		int result = mp3dec_load(&mp3d, filepath.string().c_str(), &info, NULL, NULL); //MP3 info stores all the decoded results
		if(result < 0) //Returns empty if fails to load the file
		{
			std::cout << "Failed to load mp3 file from path:" << filepath;
			return MP3Data();
		}

		data.buffer = info.buffer;//Stores the MP3 decoded buffer data into pcmData
		data.size = info.samples * sizeof(mp3d_sample_t);//Calculates the size of the buffer
		data.sampleRate = info.hz;//MP3 sample rate
		data.channels = info.channels;//Mono/stereo sound

		return data;
	}

	OggData AudioProcessor::ProcessOggData(std::filesystem::path& filepath)
	{
		FILE* file = fopen(filepath.string().c_str(), "rb");//Open file so vorbis can read it

		OggData data = OggData();//Empty oggdata struct
		OggVorbis_File vfile;//Vorbis file
		if(ov_open_callbacks(file, &vfile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		{
			std::cout << "Failed to open" << filepath << std::endl;
			return OggData();
		}

		vorbis_info* info = ov_info(&vfile, -1);//Ogg file info
		data.sampleRate = info->rate;//Ogg sample rate
		data.channels = info->channels;//Ogg channels mono/stereo
		data.samples = ov_pcm_total(&vfile, -1);//Ogg samples
		data.size = 2 * data.channels * data.samples; //Tried to use it without increasing the size but an access violation runtime error occurs so I multiplied it by 2
		//Probably uses 2 bytes per sample
		uint8_t* buf = new uint8_t[data.size];//This will be used as the buffer data to send for OpenAL to use
		uint8_t* bufferPtr = buf;//Bufferptr which will move as vorbis reads the bytes (Walker pointer)
		int eof = 0;
		while(!eof)
		{
			int section;
			long length = ov_read(&vfile, (char*)bufferPtr, 4096, 0, 2, 1, &section);//Read bytes
			bufferPtr += length;//Fill the buffer with the bytes
			if(length == 0)
			{
				eof = 1;
			}
			else if(length < 0)
			{
				if (length == OV_EBADLINK) std::cout << "Corrupt bitstream section";
				return OggData();
			}
		}
		data.buffer = buf;

		ov_clear(&vfile);
		fclose(file);

		return data;
	}
}

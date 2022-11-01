/*\file AudioProcessor.cpp*/
#include "Audio/AudioProcessor.h"

WavData AudioProcessor::ProcessWavData(std::string& filepath)
{
	WavData data = WavData(); //Empty struct
	drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(filepath.c_str(), &data.channels, &data.sampleRate, &data.totalPCMFrameCount, nullptr);
	if(pSampleData == NULL)
	{
		std::cout << "Failed to load wav file from path: " << filepath;
		drwav_free(pSampleData, nullptr);
		return WavData();
	}
	data.pcmData.resize(size_t(data.totalPCMFrameCount * data.channels) * 2); //Adjusts the size to fit the pcm data
	std::memcpy(data.pcmData.data(), pSampleData, data.pcmData.size());//Copies the pcm data to the vector
	drwav_free(pSampleData, nullptr);//Frees the data
	return data;
}

MP3Data AudioProcessor::ProcessMP3Data(std::string& filepath)
{
	return MP3Data();
}

OggData AudioProcessor::ProcessOggData(std::string& filepath)
{
	return OggData();
}

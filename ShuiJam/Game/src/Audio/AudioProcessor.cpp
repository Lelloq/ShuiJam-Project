/*\file AudioProcessor.cpp*/
#include "pch.h"
#include "Audio/AudioProcessor.h"

WavData AudioProcessor::ProcessWavData(std::string& filepath)
{
	WavData data = WavData(); //Empty struct
	float* pSampleData = drwav_open_file_and_read_pcm_frames_f32(filepath.c_str(), &data.channels, &data.sampleRate, &data.totalPCMFrameCount, NULL);
	if(pSampleData == NULL)
	{
		std::cout << "Failed to load wav file from path: " << filepath;
		return;
	}
	data.pcmData.resize(size_t(data.totalPCMFrameCount * data.channels)); //Adjusts the size to fit the pcm data
	std::memcpy(data.pcmData.data(), pSampleData, data.pcmData.size());//Copies the pcm data to the vector
	drwav_free(pSampleData, NULL);//Frees the data
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

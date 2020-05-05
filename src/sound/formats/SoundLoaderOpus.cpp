#include "sound/formats/SoundLoaderOpus.hpp"
#include <opus/opusfile.h>
#include <spdlog/spdlog.h>


/*bool magmatic::sound::SoundLoaderOpus::registered_ = magmatic::sound::SoundLoader::registerLoader(
		SoundLoaderOpus::factoryName(),
		std::make_unique<SoundLoaderOpus>()
);*/

std::shared_ptr<magmatic::sound::SoundBuffer> magmatic::sound::SoundLoaderOpus::load(const std::filesystem::path& path)
{
	if(!std::filesystem::exists(path))
	{
		spdlog::error("Magmatic: SoundLoader file doesn't exist: {}", path.string());
		throw std::runtime_error("SoundLoader file doesn't exist");
	}

	int error = 0;
	OggOpusFile* ogg_file = op_open_file(path.string().c_str(), &error);

	if(error)
	{
		spdlog::error("Magmatic: Failed to decode Opus file");
		throw std::runtime_error("Failed to decode Opus file");
	}

	const auto channel_count = op_channel_count(ogg_file, -1);
	const auto pcm_length = op_pcm_total(ogg_file, -1);

	if(channel_count > 2)
	{
		spdlog::error("Magmatic: SoundLoader files with more that 2 channels_ not supported");
		throw std::runtime_error("SoundLoader file formats not supported");
	}

	const auto format = channel_count == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	size_t sample_read = 0;

	auto buffer = new int16_t[pcm_length*channel_count];

	ALuint buffer_ID;
	alGenBuffers(1, &buffer_ID);
	if (error = alGetError(); error != AL_NO_ERROR)
	{
		delete[] buffer;
		spdlog::error("Magmatic: Failed to create sound buffer");
		throw std::runtime_error("Failed to create sound buffer");
	}

	while(sample_read < pcm_length)
	{
		ogg_int64_t temp_read = op_read(ogg_file, buffer + sample_read * channel_count, pcm_length * channel_count, nullptr);
		if(temp_read < 0)
		{
			spdlog::error("Magmatic: Error while decoding opus file");
			throw std::runtime_error("Error while decoding opus file");
		}
		sample_read += temp_read;
	}
	op_free(ogg_file);

	alBufferData(buffer_ID, format, buffer, sample_read * channel_count * sizeof(int16_t), 48000);
	if (error = alGetError(); error != AL_NO_ERROR)
	{
		spdlog::error("Magmatic: Failed to transfer audio to buffer");
		throw std::runtime_error("Failed to transfer audio to buffer");
	}

	return SoundLoaderConcrete::fromID(buffer_ID);
}

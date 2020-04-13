#include "sound/formats/SoundLoaderOgg.hpp"
#include <spdlog/spdlog.h>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <fstream>

namespace {
	constexpr size_t buffer_size = 32768;
	constexpr int endian = 0;//little endian
}

bool magmatic::sound::SoundLoaderOgg::registered = magmatic::sound::Sound::register_format(
		magmatic::sound::SoundLoaderOgg::factoryName(),
		magmatic::sound::SoundLoaderOgg::createLoader
);

std::shared_ptr<magmatic::sound::SoundBuffer> magmatic::sound::SoundLoaderOgg::open(const std::filesystem::path& path)
{
	if(!std::filesystem::exists(path))
	{
		spdlog::error("Magamtic: Ogg file doesn't exist: {}", path.string());
		throw std::runtime_error("Ogg file doesn't exist");
	}

	OggVorbis_File ogg_file;

	if(ov_fopen(path.c_str(), &ogg_file) != 0)
	{
		spdlog::error("Magmatic: Failed to decode ogg file");
		throw std::runtime_error("Failed to decode ogg file");
	}

	const auto info = ov_info(&ogg_file, -1);
	const auto format = info->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	const auto sample_rate = info->rate;

	char temp_buffer[buffer_size];
	size_t readed_bytes = 0;
	int bit_stream;

	ALuint buffer_ID;
	alGenBuffers(1, &buffer_ID);
	if (auto error = alGetError(); error != AL_NO_ERROR)
	{
		spdlog::error("Magmatic: Failed to create sound buffer");
		throw std::runtime_error("Failed to create sound buffer");
	}

	std::vector<char> buffer;

	do
	{
		readed_bytes = ov_read(&ogg_file, temp_buffer, buffer_size, endian, 2, 1, &bit_stream);
		buffer.insert(buffer.end(), temp_buffer, temp_buffer+readed_bytes);
	}
	while(readed_bytes > 0);

	ov_clear(&ogg_file);

	alBufferData(buffer_ID, format, buffer.data(), static_cast<ALsizei>(buffer.size()), sample_rate);
	if (auto error = alGetError(); error != AL_NO_ERROR)
	{
		spdlog::error("Magmatic: Failed to transfer audio to buffer");
		throw std::runtime_error("Failed to transfer audio to buffer");
	}

	return SoundLoader::fromID(buffer_ID);
}

std::unique_ptr<magmatic::sound::SoundLoader> magmatic::sound::SoundLoaderOgg::createLoader()
{
	return std::make_unique<SoundLoaderOgg>();
}

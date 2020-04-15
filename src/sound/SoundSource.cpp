#include "sound/SoundSource.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>


magmatic::sound::SoundSource::SoundSource(glm::vec3 pos)
{
	alGenSources(1, &source);
	if (auto error = alGetError(); error != AL_NO_ERROR)
	{
		spdlog::error("Magmatic: Failed to create sound source");
		throw std::runtime_error("Failed to create sound source");
	}

	alSourcef(source, AL_PITCH,1);
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(source, AL_LOOPING, AL_FALSE);
}

magmatic::sound::SoundSource::~SoundSource()
{
	alDeleteSources(1, &source);
}

void magmatic::sound::SoundSource::setSound(std::shared_ptr<SoundBuffer> buffer) const noexcept
{
	alSourcei(source, AL_BUFFER, buffer->buffer_ID);
}

void magmatic::sound::SoundSource::play() const noexcept
{
	#if !defined(NDEBUG)

	#endif

	alSourcePlay(source);
}

void magmatic::sound::SoundSource::wait() const noexcept
{
	ALint state = AL_PLAYING;

	while(state == AL_PLAYING)
	{
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
}

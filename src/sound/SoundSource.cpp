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

void magmatic::sound::SoundSource::setSound(std::shared_ptr<SoundBuffer> buffer) noexcept
{
	#if !defined(NDEBUG)
	if(!buffer)
	{
		spdlog::warn("Magmatic: Set empty sound buffer");
	}
	#endif
	curr_buffer = buffer;
	alSourcei(source, AL_BUFFER, buffer->buffer_ID);
}

void magmatic::sound::SoundSource::play() const noexcept
{
	#if !defined(NDEBUG)
		if(!curr_buffer)
		{
			spdlog::warn("Magmatic: Trying to play source with empty buffer");
		}
	#endif

	alSourcePlay(source);
}

void magmatic::sound::SoundSource::wait() const noexcept
{
	//todo: add wait timeout
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	#if !defined(NDEBUG)
		if(state != AL_PLAYING)
		{
			spdlog::warn("Magmatic: Trying to wait on not playing source");
		}
	#endif

	while(state == AL_PLAYING)
	{
		std::this_thread::yield();
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
}

void magmatic::sound::SoundSource::play(std::shared_ptr<SoundBuffer> buffer) noexcept
{
	setSound(buffer);
	play();
}

void magmatic::sound::SoundSource::pause() const noexcept
{
	#if !defined(NDEBUG)
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if(state != AL_PLAYING)
	{
		spdlog::warn("Magmatic: Trying to pause stopped or paused source");
	}
	#endif
	alSourcePause(source);
}

void magmatic::sound::SoundSource::stop() const noexcept
{
	#if !defined(NDEBUG)
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if(state != AL_PLAYING)
	{
		spdlog::warn("Magmatic: Trying to stop not playing source");
	}
	#endif
	alSourceStop(source);
}

magmatic::sound::SoundSource::State magmatic::sound::SoundSource::state() const noexcept
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	switch (state)
	{
		case AL_PLAYING:
			return State::Playing;
		case AL_PAUSED:
			return State::Paused;
		default:
			return State::Idle;
	}
}

void magmatic::sound::SoundSource::setPosition(glm::vec3 pos) const noexcept
{
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void magmatic::sound::SoundSource::setVelocity(glm::vec3 vec) const noexcept
{
	alSource3f(source, AL_VELOCITY, vec.x, vec.y, vec.z);
}

void magmatic::sound::SoundSource::setDirection(glm::vec3 dir) const noexcept
{
	alSource3f(source, AL_DIRECTION, dir.x, dir.y, dir.z);
}

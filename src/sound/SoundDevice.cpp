#include "sound/SoundDevice.hpp"
#include <AL/alc.h>
#include <spdlog/spdlog.h>
#include <AL/al.h>

void magmatic::sound::SoundDevice::ALCdeviceDeleter::operator()(ALCdevice* pointer) noexcept
{
	alcCloseDevice(pointer);
}

void magmatic::sound::SoundDevice::ALCcontextDeleter::operator()(ALCcontext* pointer) noexcept
{
	if(alcGetCurrentContext() == pointer)
	{
		alcMakeContextCurrent(nullptr);
	}
	alcDestroyContext(pointer);
}

magmatic::sound::SoundDevice::SoundDevice(const char* device)
:alc_device(alcOpenDevice(device)), alc_context(alcCreateContext(alc_device.get(), nullptr))
{
	if(!alc_device)
	{
		spdlog::error("Magmatic: Failed to open audio device");
		throw std::runtime_error("Failed to open audio device");
	}
	if(!alc_context)
	{
		spdlog::error("Magmatic: Failed to create context");
		throw std::runtime_error(" Failed to create context");
	}
	if(!alcMakeContextCurrent(alc_context.get()))
	{
		spdlog::error("Magmatic: Failed to set current context");
		throw std::runtime_error(" Failed to set current context");
	}
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

std::vector<std::string> magmatic::sound::SoundDevice::enumerateDevices() noexcept
{
	const ALCchar* devices;
	devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);

	const ALCchar* tmp = devices;

	std::vector<std::string> retval;

	do
	{
		retval.emplace_back(std::string(tmp));
		tmp += retval.back().size()+1;
	}
	while(*(tmp+1) != '\0');

	return retval;
}

void magmatic::sound::SoundDevice::setListenerPosition(glm::vec3 pos) const noexcept
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void magmatic::sound::SoundDevice::setListenerGain(float gain) const
{
	if(gain < 0.0f)
	{
		spdlog::error("Magmatic: Gain must be positive");
		throw std::invalid_argument("Gain must be positive");
	}
	alListenerf(AL_GAIN, gain);
}

void magmatic::sound::SoundDevice::setListenerVelocity(glm::vec3 vel) const noexcept
{
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

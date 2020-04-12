#include "sound/SoundDevice.hpp"
#include <AL/alc.h>

void magmatic::sound::AudioDevice::ALCdeviceDeleter::operator()(ALCdevice* pointer) noexcept
{
	alcCloseDevice(pointer);
}

void magmatic::sound::AudioDevice::ALCcontextDeleter::operator()(ALCcontext* pointer) noexcept
{
	if(alcGetCurrentContext() == pointer)
	{
		alcMakeContextCurrent(nullptr);
	}
	alcDestroyContext(pointer);
}

magmatic::sound::AudioDevice::AudioDevice()
:alc_device(alcOpenDevice(nullptr)), alc_context(alcCreateContext(alc_device.get(), nullptr))
{
	// todo: check errors
}

std::vector<std::string> magmatic::sound::AudioDevice::enumerateDevices() noexcept
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

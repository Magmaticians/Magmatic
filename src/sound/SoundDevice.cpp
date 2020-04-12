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

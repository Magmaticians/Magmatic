#include "sound/SoundBuffer.hpp"
#include <iostream>

magmatic::sound::SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &buffer_ID);
}
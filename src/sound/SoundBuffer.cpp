#include "sound/SoundBuffer.hpp"

magmatic::sound::SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &buffer_ID);
}
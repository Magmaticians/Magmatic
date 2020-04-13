#include "sound/formats/SoundLoader.hpp"

std::shared_ptr<magmatic::sound::SoundBuffer> magmatic::sound::SoundLoader::fromID(ALuint buffer_ID)
{
	return std::shared_ptr<SoundBuffer>(new SoundBuffer(buffer_ID));
}

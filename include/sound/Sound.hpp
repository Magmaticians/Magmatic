#ifndef MAGMATIC_SOUND_HPP
#define MAGMATIC_SOUND_HPP

#include "utils/factory/FileLoaderFactory.hpp"
#include "sound/SoundBuffer.hpp"

namespace magmatic::sound
{
	using Sound = magmatic::utils::FileLoaderFactory<SoundBuffer>;
}

#endif //MAGMATIC_SOUND_HPP

#ifndef MAGMATIC_SOUNDLOADER_HPP
#define MAGMATIC_SOUNDLOADER_HPP

#include "utils/factory/FileLoaderFactory.hpp"
#include "sound/SoundBuffer.hpp"

namespace magmatic::sound
{
	using SoundLoader = magmatic::utils::FileLoaderFactory<SoundBuffer>;
}

#endif //MAGMATIC_SOUNDLOADER_HPP

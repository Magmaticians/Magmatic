#ifndef MAGMATIC_SOUNDLOADER_HPP
#define MAGMATIC_SOUNDLOADER_HPP

#include "sound/Sound.hpp"
#include "sound/SoundBuffer.hpp"
#include <filesystem>
#include <AL/al.h>
#include <memory>


namespace magmatic::sound
{
	class SoundLoader
	{
	public:
		SoundLoader() = default;

		virtual ~SoundLoader() = default;

		virtual std::shared_ptr<SoundBuffer> open(const std::filesystem::path& path) = 0;

	protected:
		static std::shared_ptr<SoundBuffer> fromID(ALuint buffer_ID);
	};
}


#endif //MAGMATIC_SOUNDLOADER_HPP

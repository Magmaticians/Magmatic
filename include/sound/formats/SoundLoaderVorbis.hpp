#ifndef MAGMATIC_SOUNDLOADERVORBIS_HPP
#define MAGMATIC_SOUNDLOADERVORBIS_HPP

#include "sound/formats/SoundLoader.hpp"
#include "sound/SoundBuffer.hpp"
#include <filesystem>


namespace magmatic::sound
{
	class SoundLoaderVorbis: public SoundLoader
	{
	public:
		std::shared_ptr<SoundBuffer> open(const std::filesystem::path& path) override;
		static bool registered;
		static std::string factoryName() { return "VORBIS"; }

		static std::unique_ptr<SoundLoader> createLoader();
	};
}


#endif //MAGMATIC_SOUNDLOADERVORBIS_HPP

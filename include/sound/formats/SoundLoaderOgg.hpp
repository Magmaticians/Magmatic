#ifndef MAGMATIC_SOUNDLOADEROGG_HPP
#define MAGMATIC_SOUNDLOADEROGG_HPP

#include "sound/formats/SoundLoader.hpp"
#include "sound/SoundBuffer.hpp"

namespace magmatic::sound
{
	class SoundLoaderOgg: public SoundLoader
	{
	public:
		std::shared_ptr<SoundBuffer> open(const std::filesystem::path& path) override;
		static bool registered;
		static std::string factoryName() { return "OGG"; }

		static std::unique_ptr<SoundLoader> createLoader();
	};
}


#endif //MAGMATIC_SOUNDLOADEROGG_HPP

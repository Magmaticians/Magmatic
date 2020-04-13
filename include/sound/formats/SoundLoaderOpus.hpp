#ifndef MAGMATIC_SOUNDLOADEROPUS_HPP
#define MAGMATIC_SOUNDLOADEROPUS_HPP

#include "sound/SoundBuffer.hpp"
#include "sound/formats/SoundLoader.hpp"
#include <filesystem>
#include <memory>


namespace magmatic::sound
{
	class SoundLoaderOpus: 	public SoundLoader
	{
	public:
		std::shared_ptr<SoundBuffer> open(const std::filesystem::path& path) override;
		static bool registered;
		static std::string factoryName() { return "OPUS"; }

		static std::unique_ptr<SoundLoader> createLoader();
	};
}


#endif //MAGMATIC_SOUNDLOADEROPUS_HPP

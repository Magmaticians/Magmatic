#ifndef MAGMATIC_SOUNDLOADEROPUS_HPP
#define MAGMATIC_SOUNDLOADEROPUS_HPP

#include "sound/SoundBuffer.hpp"
#include "sound/formats/SoundLoaderConcrete.hpp"
#include <filesystem>
#include <memory>


namespace magmatic::sound
{
	class SoundLoaderOpus: 	public SoundLoaderConcrete
	{
	public:
		std::shared_ptr<SoundBuffer> load(const std::filesystem::path& path) override;
		static std::string factoryName() { return "OPUS"; }

	private:
		static bool registered_;

	};
}


#endif //MAGMATIC_SOUNDLOADEROPUS_HPP

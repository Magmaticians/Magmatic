#ifndef MAGMATIC_SOUNDLOADERVORBIS_HPP
#define MAGMATIC_SOUNDLOADERVORBIS_HPP

#include "sound/formats/SoundLoaderConcrete.hpp"
#include "sound/SoundBuffer.hpp"
#include <filesystem>


namespace magmatic::sound
{
	class SoundLoaderVorbis: public SoundLoaderConcrete
	{
	public:
		std::shared_ptr<SoundBuffer> load(const std::filesystem::path& path) override;
		static std::string factoryName() { return "VORBIS"; }

	private:
		static bool registered_;
	};
}


#endif //MAGMATIC_SOUNDLOADERVORBIS_HPP

#ifndef MAGMATIC_SOUND_HPP
#define MAGMATIC_SOUND_HPP

#include "sound/SoundBuffer.hpp"
#include <memory>
#include <filesystem>
#include <map>
#include <functional>


namespace magmatic::sound
{
	class SoundLoader;

	class Sound
	{
	public:
		using TSoundLoaderCreator = std::unique_ptr<SoundLoader>(*)();
		using TSoundLoader = std::unique_ptr<SoundLoader>;
	public:
		Sound() = delete;

		static bool register_format(const std::string& name, TSoundLoaderCreator loader);

		static std::shared_ptr<SoundBuffer> open(const std::string& name, const std::filesystem::path& path);

	private:
		static std::unordered_map<std::string, TSoundLoader> loaders;
	};
}


#endif //MAGMATIC_SOUND_HPP

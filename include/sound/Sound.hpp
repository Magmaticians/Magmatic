#ifndef MAGMATIC_SOUND_HPP
#define MAGMATIC_SOUND_HPP

#include "sound/formats/SoundLoader.hpp"
#include "sound/SoundBuffer.hpp"
#include <memory>
#include <map>
#include <functional>


namespace magmatic::sound
{
	class Sound
	{
	public:
		using TSoundBuffer = std::shared_ptr<SoundBuffer>;
		using TSoundLoader = std::function<TSoundBuffer(const std::filesystem::path&)>;
	public:
		Sound() = delete;

		static bool register_format(const std::string& name, TSoundLoader loader);

		static std::shared_ptr<SoundBuffer> open(const std::string& name, const std::filesystem::path& path);

	private:
		static std::map<std::string, TSoundLoader> loaders;
	};
}


#endif //MAGMATIC_SOUND_HPP

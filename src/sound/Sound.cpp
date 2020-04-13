#include "sound/Sound.hpp"

bool magmatic::sound::Sound::register_format(const std::string& name, TSoundLoader loader)
{
	if(auto it = loaders.find(name); it == std::end(loaders))
	{
		loaders[name] = loader;
		return true;
	}
	return false;
}

std::shared_ptr<magmatic::sound::SoundBuffer> magmatic::sound::Sound::open(
		const std::string& name,
		const std::filesystem::path& path
		)
{
	if(auto it = loaders.find(name); it != loaders.end())
	{
		return it->second(path);
	}
	return nullptr;
}

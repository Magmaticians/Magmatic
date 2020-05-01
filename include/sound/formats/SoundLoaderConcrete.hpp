#ifndef MAGMATIC_SOUNDLOADERCONCRETE_HPP
#define MAGMATIC_SOUNDLOADERCONCRETE_HPP

#include "sound/SoundLoader.hpp"
#include "utils/factory/FileLoaderFactory.hpp"

namespace magmatic::sound
{
class SoundLoaderConcrete : public utils::ConcreteLoader<SoundLoader>
	{
	protected:
		std::shared_ptr<SoundBuffer> fromID(ALuint buffer_ID)
		{
			return std::shared_ptr<SoundBuffer>(new SoundBuffer(buffer_ID));
		}
	};
}


#endif //MAGMATIC_SOUNDLOADERCONCRETE_HPP

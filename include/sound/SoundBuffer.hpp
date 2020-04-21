#ifndef MAGMATIC_SOUNDBUFFER_HPP
#define MAGMATIC_SOUNDBUFFER_HPP

#include <AL/al.h>

namespace magmatic::sound
{
	class SoundBuffer
	{
		friend class SoundLoader;
		explicit SoundBuffer(ALuint buffer_ID) noexcept : buffer_ID(buffer_ID) {};

	public:
		~SoundBuffer();

		const ALuint buffer_ID;
	};
}


#endif //MAGMATIC_SOUNDBUFFER_HPP

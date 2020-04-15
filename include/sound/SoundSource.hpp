#ifndef MAGMATIC_SOUNDSOURCE_HPP
#define MAGMATIC_SOUNDSOURCE_HPP

#include "sound/SoundBuffer.hpp"
#include <AL/al.h>
#include <glm/glm.hpp>
#include <memory>

namespace magmatic::sound
{
	class SoundSource
	{
	public:
		enum State
		{
			Idle = 0,

		};
	private:
		ALuint source;
		std::shared_ptr<SoundBuffer> buffer { nullptr };

	public:
		SoundSource(glm::vec3 pos = {0, 0, 0});
		~SoundSource();

		SoundSource(const SoundSource&) = delete;
		SoundSource& operator=(const SoundSource&) = delete;

		void setSound(std::shared_ptr<SoundBuffer> buffer) const noexcept;
		void play() const noexcept;
		void play(std::shared_ptr<SoundBuffer> buffer) const noexcept ;
		void pause() const noexcept;
		void stop() const noexcept;

		void wait() const noexcept;

	};
}


#endif //MAGMATIC_SOUNDSOURCE_HPP

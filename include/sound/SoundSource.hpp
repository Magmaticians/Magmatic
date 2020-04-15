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
			Playing = 1,
			Paused = 2
		};
	private:
		ALuint source;
		std::shared_ptr<SoundBuffer> curr_buffer { nullptr };

	public:
		SoundSource(glm::vec3 pos = {0, 0, 0});
		~SoundSource();

		SoundSource(const SoundSource&) = delete;
		SoundSource& operator=(const SoundSource&) = delete;

		void setPosition(glm::vec3 pos) const noexcept;
		void setVelocity(glm::vec3 vec) const noexcept;
		void setDirection(glm::vec3 dir) const noexcept;

		void setSound(std::shared_ptr<SoundBuffer> buffer) noexcept;
		void play() const noexcept;
		void play(std::shared_ptr<SoundBuffer> buffer) noexcept ;
		void pause() const noexcept;
		void stop() const noexcept;

		void wait() const noexcept;

		State state() const noexcept;

	};
}


#endif //MAGMATIC_SOUNDSOURCE_HPP

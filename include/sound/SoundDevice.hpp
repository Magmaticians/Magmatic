#ifndef MAGMATIC_SOUNDDEVICE_HPP
#define MAGMATIC_SOUNDDEVICE_HPP

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace magmatic::sound
{
	class SoundDevice
	{

	public:
		struct ALCdeviceDeleter
		{
			void operator()(ALCdevice* pointer) noexcept;
		};

		struct ALCcontextDeleter
		{
			void operator()(ALCcontext* pointer) noexcept;
		};

		SoundDevice() : SoundDevice(nullptr) {};
		explicit SoundDevice(const std::string& device) : SoundDevice(device.c_str()) {};

		SoundDevice(const SoundDevice&) = delete;
		SoundDevice& operator=(const SoundDevice&) = delete;

		static std::vector<std::string> enumerateDevices() noexcept;

		void setListenerPosition(glm::vec3 pos) const noexcept;
		void setListenerVelocity(glm::vec3 vel) const noexcept;
		void setListenerGain(float gain) const;


	private:
		explicit SoundDevice(const char* device);

		std::unique_ptr<ALCdevice, ALCdeviceDeleter> alc_device;
		std::unique_ptr<ALCcontext, ALCcontextDeleter> alc_context;
	};
}

#endif //MAGMATIC_SOUNDDEVICE_HPP

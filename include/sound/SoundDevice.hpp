#ifndef MAGMATIC_SOUNDDEVICE_HPP
#define MAGMATIC_SOUNDDEVICE_HPP

#include <memory>
#include <vector>
#include <string>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace magmatic::sound
{
	class AudioDevice
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

		AudioDevice() : AudioDevice(nullptr) {};
		AudioDevice(const std::string& device) : AudioDevice(device.c_str()) {};

		AudioDevice(const AudioDevice&) = delete;
		AudioDevice& operator=(const AudioDevice&) = delete;

		static std::vector<std::string> enumerateDevices() noexcept;

	private:
		AudioDevice(const char* device);

		std::unique_ptr<ALCdevice, ALCdeviceDeleter> alc_device;
		std::unique_ptr<ALCcontext, ALCcontextDeleter> alc_context;
	};
}

#endif //MAGMATIC_SOUNDDEVICE_HPP

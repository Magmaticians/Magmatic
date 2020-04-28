#ifndef MAGMATIC_IMAGE_HPP
#define MAGMATIC_IMAGE_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic::render {
	class Image {
		friend class LogicalDevice;

		vk::UniqueImage image;
		vk::UniqueDeviceMemory memory;

		explicit Image(vk::UniqueImage image, vk::UniqueDeviceMemory memory) :
				image(std::move(image)), memory(std::move(memory)) {}
	public:

		Image(Image&) = delete;
		Image &operator=(Image &) = delete;
	};
}

#endif //MAGMATIC_IMAGE_HPP

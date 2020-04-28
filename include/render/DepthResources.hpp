#ifndef MAGMATIC_DEPTHRESOURCES_HPP
#define MAGMATIC_DEPTHRESOURCES_HPP

#include <vulkan/vulkan.hpp>
#include "PhysicalDevice.hpp"

namespace magmatic::render {
	class DepthResources {
		friend class LogicalDevice;

		explicit DepthResources(vk::UniqueImage image, vk::UniqueDeviceMemory memory, vk::UniqueImageView imageView, vk::Format format) :
			image(std::move(image)),
			memory(std::move(memory)),
			imageView(std::move(imageView)),
			format(format){};

	public:
		vk::UniqueImage image;
		vk::UniqueDeviceMemory memory;
		vk::UniqueImageView imageView;
		vk::Format format;

		DepthResources(DepthResources
		&) = delete;

		DepthResources &operator=(DepthResources &) = delete;
	};
}

#endif //MAGMATIC_DEPTHRESOURCES_HPP

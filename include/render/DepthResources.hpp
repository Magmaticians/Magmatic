#ifndef MAGMATIC_DEPTHRESOURCES_HPP
#define MAGMATIC_DEPTHRESOURCES_HPP

#include <vulkan/vulkan.hpp>
#include "PhysicalDevice.hpp"
#include "CommandPool.hpp"
#include "Image.hpp"

namespace magmatic::render {
	class DepthResources {

	public:
		vk::Format format;
		Image image;
		vk::UniqueImageView imageView;

		DepthResources(const LogicalDevice& l_device, vk::Extent2D extent, const CommandPool& commandPool);

		DepthResources(DepthResources&) = delete;
		DepthResources &operator=(DepthResources&) = delete;

	private:
		[[nodiscard]] vk::Format findDepthFormat(const LogicalDevice& l_device) const;
		[[nodiscard]] magmatic::render::Image createDepthImage(const LogicalDevice& l_device, vk::Extent2D extent) const;
	};
}

#endif //MAGMATIC_DEPTHRESOURCES_HPP

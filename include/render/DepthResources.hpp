#ifndef MAGMATIC_DEPTHRESOURCES_HPP
#define MAGMATIC_DEPTHRESOURCES_HPP

#include <vulkan/vulkan.hpp>
#include "PhysicalDevice.hpp"
#include "CommandPool.hpp"
#include "Image.hpp"

namespace magmatic::render {
	class DepthResources {
	public:

		DepthResources(const LogicalDevice& l_device, vk::Extent2D extent, const CommandPool& commandPool);

		DepthResources(const DepthResources&) = delete;
		DepthResources& operator=(const DepthResources&) = delete;

		DepthResources(DepthResources&& rhs) noexcept = default;
		DepthResources& operator=(DepthResources&& rhs) noexcept = default;

		~DepthResources() = default;

		[[nodiscard]] const vk::Format& getFormat() const;
		[[nodiscard]] const Image& getImage() const;
		[[nodiscard]] const vk::UniqueImageView& getImageView() const;

	private:
		vk::Format format;
		Image image;
		vk::UniqueImageView imageView;

		[[nodiscard]] vk::Format findDepthFormat(const LogicalDevice& l_device) const;
		[[nodiscard]] magmatic::render::Image createDepthImage(const LogicalDevice& l_device, vk::Extent2D extent) const;
	};
}

#endif //MAGMATIC_DEPTHRESOURCES_HPP

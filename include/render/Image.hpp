#ifndef MAGMATIC_IMAGE_HPP
#define MAGMATIC_IMAGE_HPP

#include "render/LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>
#include "CommandPool.hpp"

namespace magmatic::render {
	class Image {

		vk::UniqueImage image;
		vk::UniqueDeviceMemory memory;
		vk::Format image_format;
	public:
		Image(const LogicalDevice& l_device, vk::Extent2D extent, vk::Format format, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage, const vk::MemoryPropertyFlags& memProps);

		Image(Image&) = delete;
		Image &operator=(Image &) = delete;

		Image(Image&& rhs) noexcept: image(std::move(rhs.image)), memory(std::move(rhs.memory)), image_format(rhs.image_format) {}
		Image &operator=(Image && rhs) noexcept;

		[[nodiscard]] const vk::UniqueImage& getImage() {
			return image;
		}
		[[nodiscard]] const vk::UniqueDeviceMemory& getMemory() {
			return memory;
		}

		void transitionImageLayout(vk::ImageLayout old_layout,
				vk::ImageLayout new_layout, const CommandPool& command_pool);

		[[nodiscard]] vk::UniqueImageView createImageView(const vk::ImageAspectFlags& aspectFlags, const vk::ComponentMapping& compMapping);

		[[nodiscard]] static vk::UniqueImageView createImageView(const LogicalDevice& l_device, const vk::Image& image, const vk::Format& format, const vk::ImageAspectFlags& aspectFlags, const vk::ComponentMapping& compMapping);
	};
}

#endif //MAGMATIC_IMAGE_HPP

#ifndef MAGMATIC_IMAGE_HPP
#define MAGMATIC_IMAGE_HPP

#include "render/LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>
#include "CommandPool.hpp"

namespace magmatic::render {
	class Image {
	public:
		Image(const LogicalDevice& l_device, vk::Extent2D extent, vk::Format format, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage, const vk::MemoryPropertyFlags& memProps);

		Image(const Image&) = delete;
		Image &operator=(const Image &) = delete;

		Image(Image&& rhs) noexcept = default;
		Image &operator=(Image && rhs) noexcept = default;

		~Image() = default;

		[[nodiscard]] const vk::UniqueImage& getImage();

		[[nodiscard]] const vk::UniqueDeviceMemory& getMemory();

		void transitionImageLayout(
				vk::ImageLayout old_layout,
				vk::ImageLayout new_layout,
				const CommandPool& command_pool
				);

		[[nodiscard]] vk::UniqueImageView createImageView(
				const vk::ImageAspectFlags& aspect_flags,
				const vk::ComponentMapping& component_mapping
				);

		[[nodiscard]] static vk::UniqueImageView createImageView(
				const LogicalDevice& l_device,
				const vk::Image& image,
				const vk::Format& format,
				const vk::ImageAspectFlags& aspect_flags,
				const vk::ComponentMapping& component_mapping
				);

	private:
		vk::UniqueDeviceMemory memory;
		vk::UniqueImage image;
		vk::Format image_format;
	};
}

#endif //MAGMATIC_IMAGE_HPP

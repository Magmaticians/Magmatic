#include "render/utils.hpp"
#include "render/DepthResources.hpp"

vk::Format magmatic::render::DepthResources::findDepthFormat(const LogicalDevice& l_device) const {
	return utils::findSupportedFormat(
			{vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
			vk::ImageTiling::eOptimal,
			vk::FormatFeatureFlagBits::eDepthStencilAttachment,
			l_device.getPhysicalDevice());
}

magmatic::render::Image magmatic::render::DepthResources::createDepthImage(const LogicalDevice& l_device, vk::Extent2D extent) const {

	return Image(l_device,
	             extent,
	             format,
	             vk::ImageTiling::eOptimal,
	             vk::ImageUsageFlagBits::eDepthStencilAttachment,
	             vk::MemoryPropertyFlagBits::eDeviceLocal);
}

magmatic::render::DepthResources::DepthResources(const LogicalDevice& l_device, vk::Extent2D extent, const CommandPool& commandPool): format(findDepthFormat(l_device)), image(createDepthImage(l_device, extent)){
	const auto& handle = l_device.getHandle();

	imageView = image.createImageView(vk::ImageAspectFlagBits::eDepth, {});

	image.transitionImageLayout(vk::ImageLayout::eUndefined,
	                      vk::ImageLayout::eDepthStencilAttachmentOptimal,
	                      commandPool);
}

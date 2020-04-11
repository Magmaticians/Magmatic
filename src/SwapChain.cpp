#include "SwapChain.hpp"

vk::SurfaceFormatKHR
magmatic::SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
{
	if(formats.empty())
	{
		throw std::invalid_argument("Provided empty formats vector");
	}
	if(
			const auto preferred_format = std::find_if(
					formats.begin(),
					formats.end(),
					[](const vk::SurfaceFormatKHR& surface_format)
					{
						return surface_format.format == vk::Format::eB8G8R8A8Srgb
						&& surface_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
					});
			preferred_format != formats.begin())
	{
		return *preferred_format;
	}
	return formats[0];

}

vk::PresentModeKHR magmatic::SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes)
{
	if(present_modes.empty())
	{
		throw std::invalid_argument("Provided empty present modes vector");
	}
	if(std::find(present_modes.begin(), present_modes.end(), vk::PresentModeKHR::eMailbox) != present_modes.end())
	{
		return vk::PresentModeKHR::eMailbox;
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D magmatic::SwapChain::chooseSwapExtent(
		const vk::SurfaceCapabilitiesKHR& capabilities,
		uint32_t window_width, uint32_t window_height
		) noexcept
{
	if(capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}

	vk::Extent2D actual_extent;

	const auto& min_width = capabilities.minImageExtent.width;
	const auto& min_height = capabilities.minImageExtent.height;
	const auto& max_width = capabilities.maxImageExtent.width;
	const auto& max_height = capabilities.maxImageExtent.height;

	actual_extent.width = std::clamp(window_width, min_width, max_height);
	actual_extent.height = std::clamp(window_height, min_height, max_height);

	return actual_extent;
}

magmatic::SwapChain::SwapChain(
		vk::UniqueSwapchainKHR swapchain,
		const vk::UniqueDevice& device,
		const vk::Format& format,
        vk::Extent2D extent
		)
:swapchain_(std::move(swapchain)), extent(extent)
{
	images_ = device->getSwapchainImagesKHR(swapchain_.get());
	image_views_.reserve(images_.size());

	vk::ComponentMapping component_mapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subresource_range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

	for(const auto& image : images_)
	{
		vk::ImageViewCreateInfo image_view_create_info(
				vk::ImageViewCreateFlags(),
				image,
				vk::ImageViewType::e2D,
				format,
				component_mapping,
				subresource_range
				);
		image_views_.emplace_back(device->createImageViewUnique(image_view_create_info));
	}

	vk::FenceCreateInfo fence_create_info{vk::FenceCreateFlags()};
	fence = device->createFenceUnique(fence_create_info);
}

#include "render/SwapChain.hpp"

vk::SurfaceFormatKHR
magmatic::render::SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
{
	if (formats.empty())
	{
		throw std::invalid_argument("Provided empty formats vector");
	}
	if (
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

vk::PresentModeKHR magmatic::render::SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes)
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

vk::Extent2D magmatic::render::SwapChain::chooseSwapExtent(
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

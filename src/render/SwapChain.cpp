#include "render/SwapChain.hpp"
#include "render/Image.hpp"

magmatic::render::SwapChain::SwapChain(const LogicalDevice& l_device, const Surface& surface, const Window& window) {
	const auto& handle = l_device.getHandle();
	const auto& support_details = l_device.getPhysicalDevice().getSwapChainSupportDetails(surface);
	const auto& capabilities = support_details.capabilities;

	const auto surface_format = SwapChain::chooseSwapSurfaceFormat(support_details.formats);
	extent = chooseSwapExtent(capabilities, window);
	const auto present_mode = SwapChain::chooseSwapPresentMode(support_details.present_modes);

	uint32_t image_count = support_details.capabilities.minImageCount + 1;

	if(capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount)
	{
		image_count = capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR swapchain_create_info(
			vk::SwapchainCreateFlagsKHR(),
			*surface.surface,
			image_count,
			surface_format.format,
			surface_format.colorSpace,
			extent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			0,
			nullptr,
			capabilities.currentTransform,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			present_mode,
			true,
			nullptr
	);

	if(!l_device.sameQueueFamily())
	{
		uint32_t family_indices[2] = {l_device.getGraphicsQueueIndex(), l_device.getPresentQueueIndex()};

		swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
		swapchain_create_info.queueFamilyIndexCount = 2;
		swapchain_create_info.pQueueFamilyIndices = family_indices;
	}

	swapchain_ = handle->createSwapchainKHRUnique(swapchain_create_info);

	images_ = handle->getSwapchainImagesKHR(swapchain_.get());

	image_views_.reserve(images_.size());

	vk::ComponentMapping component_mapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA);

	for(const auto& image: images_) {
		image_views_.emplace_back(Image::createImageView(l_device,
		                                                image,
		                                                surface_format.format,
		                                                vk::ImageAspectFlagBits::eColor,
		                                                component_mapping));
	}

	vk::FenceCreateInfo fence_create_info{vk::FenceCreateFlags()};
	fence_ = handle->createFenceUnique(fence_create_info);
}

magmatic::render::SwapChain::SwapChain(SwapChain&& rhs) noexcept : extent(rhs.extent), images_(std::move(rhs.images_)),
	swapchain_(std::move(rhs.swapchain_)), image_views_(std::move(rhs.image_views_)), fence_(std::move(rhs.fence_)) {}
magmatic::render::SwapChain& magmatic::render::SwapChain::operator=(SwapChain&&rhs) noexcept {
	this->extent = rhs.extent;
	this->images_ = std::move(rhs.images_);
	this->swapchain_ = std::move(rhs.swapchain_);
	this->image_views_ = std::move(rhs.image_views_);
	this->fence_ = std::move(rhs.fence_);
	return *this;
}

vk::SurfaceFormatKHR magmatic::render::SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
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
		const Window& window
		) noexcept
{
	if(capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}

	vk::Extent2D actual_extent = window.getFramebufferSize();

	const auto& min_width = capabilities.minImageExtent.width;
	const auto& min_height = capabilities.minImageExtent.height;
	const auto& max_width = capabilities.maxImageExtent.width;
	const auto& max_height = capabilities.maxImageExtent.height;

	actual_extent.width = std::clamp(actual_extent.width, min_width, max_height);
	actual_extent.height = std::clamp(actual_extent.height, min_height, max_height);

	return actual_extent;
}

vk::Result magmatic::render::SwapChain::presentKHR(const LogicalDevice& l_device, const Semaphores& renderFinishedSemaphores, size_t index, uint32_t currentBuffer) const {
	return l_device.getPresentQueue().presentKHR(vk::PresentInfoKHR(
			1,
			&renderFinishedSemaphores.semaphores[index].get(),
			1,
			&swapchain_.get(),
			&currentBuffer
	));
}

vk::Result magmatic::render::SwapChain::acquireNextImageKHR(const Semaphores& imageAcquiredSemaphores,
		size_t index, uint32_t& imageIndex, uint64_t timeout) const {
	return swapchain_.getOwner().acquireNextImageKHR(
			swapchain_.get(),
			timeout,
			imageAcquiredSemaphores.semaphores[index].get(),
			nullptr,
			&imageIndex
	);
}

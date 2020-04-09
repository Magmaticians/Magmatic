#ifndef MAGMATIC_SWAPCHAIN_HPP
#define MAGMATIC_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


namespace magmatic
{
	class SwapChain
	{
	public:
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector <vk::SurfaceFormatKHR>& formats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes);
		static vk::Extent2D chooseSwapExtent(
				const vk::SurfaceCapabilitiesKHR& capabilities,
				uint32_t window_width, uint32_t window_height
				) noexcept;

		SwapChain(
				vk::UniqueSwapchainKHR swapchain,
				const vk::UniqueDevice& device,
				const vk::Format& format
		);

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		vk::UniqueSwapchainKHR swapchain_;

		std::vector<vk::Image> images_;
		std::vector<vk::UniqueImageView> image_views_;

		vk::UniqueFence fence;
	};

}

#endif //MAGMATIC_SWAPCHAIN_HPP

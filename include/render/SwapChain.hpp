#ifndef MAGMATIC_SWAPCHAIN_HPP
#define MAGMATIC_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


namespace magmatic::render
{
	class SwapChain
	{
	friend class LogicalDevice;

		explicit SwapChain(
				vk::UniqueSwapchainKHR swapchain,
				std::vector<vk::Image> images,
				std::vector<vk::UniqueImageView> image_views,
				vk::UniqueFence fence,
				vk::Extent2D extent
		) : swapchain_(std::move(swapchain)), images_(std::move(images)),
		    image_views_(std::move(image_views)), fence(std::move(fence)),
		    extent(extent) {}

	public:
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector <vk::SurfaceFormatKHR>& formats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& present_modes);
		static vk::Extent2D chooseSwapExtent(
				const vk::SurfaceCapabilitiesKHR& capabilities,
				uint32_t window_width, uint32_t window_height
				) noexcept;


		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		const vk::Extent2D extent;
		std::vector<vk::Image> images_;

	private:
		vk::UniqueSwapchainKHR swapchain_;

		std::vector<vk::UniqueImageView> image_views_;

		vk::UniqueFence fence;
	};

}

#endif //MAGMATIC_SWAPCHAIN_HPP

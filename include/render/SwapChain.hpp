#ifndef MAGMATIC_SWAPCHAIN_HPP
#define MAGMATIC_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "Surface.hpp"
#include "LogicalDevice.hpp"
#include "Semaphores.hpp"


namespace magmatic::render
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


		explicit SwapChain(const LogicalDevice& l_device, const Surface& surface, uint32_t window_width, uint32_t window_height);

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		void presentKHR(const LogicalDevice& l_device, const Semaphores& renderFinishedSemaphores, size_t index, uint32_t currentBuffer) const;

		[[nodiscard]] const std::vector<vk::UniqueImageView>& getImageViews() const {
			return image_views_;
		}

		vk::Result acquireNextImageKHR(const Semaphores& imageAcquiredSemaphores,
				size_t index, uint32_t& imageIndex, uint64_t timeout = UINT64_MAX) const;

		vk::Extent2D extent;
		std::vector<vk::Image> images_;

	private:
		vk::UniqueSwapchainKHR swapchain_;

		std::vector<vk::UniqueImageView> image_views_;

		vk::UniqueFence fence_;
	};

}

#endif //MAGMATIC_SWAPCHAIN_HPP

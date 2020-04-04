#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic
{
	class LogicalDevice
	{
	private:
		static constexpr const char* DEFAULT_EXTENSIONS[] =
				{
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};

		vk::UniqueDevice device;
		vk::Queue graphics_queue;
		vk::Queue present_queue;

	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const std::vector<std::string>& extensions,
				const Surface& surface
				);

		SwapChain createSwapchain(const vk::Extent2D& extent) const;

	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
				);
	};

}


#endif //MAGMATIC_LOGICALDEVICE_HPP

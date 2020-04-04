#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic
{
	class LogicalDevice
	{
	private:
		vk::UniqueDevice device;
		vk::Queue graphics_queue;
		vk::Queue present_queue;

	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const std::vector<std::string>& extensions,
				const Surface& surface
				);

	private:
		std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
				);
	};

}


#endif //MAGMATIC_LOGICALDEVICE_HPP

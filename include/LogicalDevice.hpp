#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic
{
	class LogicalDevice
	{
	private:
		vk::UniqueDevice device;
		vk::Queue graphics_queue;

	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const std::vector<std::string>& extensions
				);
	};
}


#endif //MAGMATIC_LOGICALDEVICE_HPP

#ifndef MAGMATIC_PHYSICALDEVICE_HPP
#define MAGMATIC_PHYSICALDEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


namespace magmatic
{
	class PhysicalDevice
	{
		friend class Instance;
	private:
		explicit PhysicalDevice(vk::PhysicalDevice physical_device);

	public:
		uint32_t calculateScore() const noexcept;

		const vk::PhysicalDevice device;
		const vk::PhysicalDeviceProperties device_properties;
		const vk::PhysicalDeviceFeatures device_features;
		const std::vector<vk::QueueFamilyProperties> queue_family_properties;
	};
}

#endif //MAGMATIC_PHYSICALDEVICE_HPP

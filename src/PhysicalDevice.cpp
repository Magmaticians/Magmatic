#include "PhysicalDevice.hpp"

magmatic::PhysicalDevice::PhysicalDevice(vk::PhysicalDevice physical_device)
:   device(physical_device),
	device_properties(physical_device.getProperties()),
	device_features(physical_device.getFeatures()),
	queue_family_properties(physical_device.getQueueFamilyProperties())
{}

uint32_t magmatic::PhysicalDevice::calculateScore() const noexcept
{
	uint32_t score = 0;
	if(!device_features.geometryShader)
	{
		return 0;
	}

	if(device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		score += 1000;
	}

	score += device_properties.limits.maxImageDimension2D;

	return score;
}

#include "PhysicalDevice.hpp"
#include <range/v3/all.hpp> // todo: import only necessary
#include "Surface.hpp"


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

std::vector<size_t> magmatic::PhysicalDevice::getGraphicQueue() const noexcept
{
	std::vector<size_t> queues;

	for(const auto& [index, family] : queue_family_properties | ranges::views::enumerate)
	{
		if(family.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			queues.emplace_back(index);
		}
	}

	return queues;
}

std::vector<size_t> magmatic::PhysicalDevice::getPresentQueue(const magmatic::Surface& surface) const noexcept
{
	std::vector<size_t> queues;

	for(auto i : ranges::views::iota(0, static_cast<int>(queue_family_properties.size())))
	{
		if(device.getSurfaceSupportKHR(i, surface.surface.get()))
		{
			queues.emplace_back(i);
		}
	}

	return queues;
}

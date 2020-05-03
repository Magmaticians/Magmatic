#include "render/PhysicalDevice.hpp"
#include "render/Surface.hpp"


magmatic::render::PhysicalDevice::PhysicalDevice(vk::PhysicalDevice physical_device)
:   device(physical_device),
	device_properties(physical_device.getProperties()),
	device_features(physical_device.getFeatures()),
	queue_family_properties(physical_device.getQueueFamilyProperties())
{}

uint32_t magmatic::render::PhysicalDevice::calculateScore() const noexcept
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

std::vector<size_t> magmatic::render::PhysicalDevice::getGraphicQueue() const noexcept
{
	std::vector<size_t> queues;

	for(size_t i = 0; i < queue_family_properties.size(); i++) {
        if(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            queues.emplace_back(i);
        }
	}

	return queues;
}

std::vector<size_t> magmatic::render::PhysicalDevice::getPresentQueue(const magmatic::render::Surface& surface) const noexcept
{
	std::vector<size_t> queues;

	for(size_t i = 0; i < queue_family_properties.size(); i++) {
        if(device.getSurfaceSupportKHR(i, *surface.surface))
        {
            queues.emplace_back(i);
        }
	}

	return queues;
}

magmatic::render::SwapChainSupportDetails
magmatic::render::PhysicalDevice::getSwapChainSupportDetails(const magmatic::render::Surface& surface) const
{
	const auto surface_capabilities = device.getSurfaceCapabilitiesKHR(surface.surface.get());

	const auto surface_format = device.getSurfaceFormatsKHR(surface.surface.get());

	const auto surface_present_modes = device.getSurfacePresentModesKHR(surface.surface.get());

	return SwapChainSupportDetails{surface_capabilities, surface_format, surface_present_modes};


}

magmatic::render::PhysicalDevice::PhysicalDevice(const magmatic::render::PhysicalDevice &rhs) noexcept :
	device(rhs.device),
	device_properties(rhs.device_properties),
	device_features(rhs.device_features),
	queue_family_properties(rhs.queue_family_properties) { }

magmatic::render::PhysicalDevice::PhysicalDevice(PhysicalDevice&& rhs) noexcept :
	device(rhs.device),
	device_properties(rhs.device_properties),
	device_features(rhs.device_features),
	queue_family_properties(rhs.queue_family_properties) { }

magmatic::render::PhysicalDevice& magmatic::render::PhysicalDevice::operator=(PhysicalDevice&& rhs) noexcept {
	this->device = rhs.device;
	this->device_properties = rhs.device_properties;
	this->device_features = rhs.device_features;
	this->queue_family_properties = rhs.queue_family_properties;
	return *this;
}




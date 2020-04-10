#include "Surface.hpp"


magmatic::Surface::Surface(vk::UniqueSurfaceKHR surface): surface(std::move(surface))
{

}

std::vector<vk::SurfaceFormatKHR> magmatic::Surface::getFormat(const magmatic::PhysicalDevice& physical_device) const
{
	return physical_device.device.getSurfaceFormatsKHR(surface.get());
}

vk::SurfaceCapabilitiesKHR magmatic::Surface::getCapabilities(const PhysicalDevice& device) const
{
	return device.device.getSurfaceCapabilitiesKHR(surface.get());
}

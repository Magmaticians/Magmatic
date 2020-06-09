#include "render/Surface.hpp"


magmatic::render::Surface::Surface(vk::UniqueSurfaceKHR surface): surface(std::move(surface))
{

}

std::vector<vk::SurfaceFormatKHR> magmatic::render::Surface::getFormat(const magmatic::render::PhysicalDevice& physical_device) const
{
	return physical_device.device.getSurfaceFormatsKHR(surface.get());
}

vk::SurfaceCapabilitiesKHR magmatic::render::Surface::getCapabilities(const PhysicalDevice& device) const
{
	return device.device.getSurfaceCapabilitiesKHR(surface.get());
}

const vk::UniqueSurfaceKHR &magmatic::render::Surface::getSurface() const
{
	return this->surface;
}

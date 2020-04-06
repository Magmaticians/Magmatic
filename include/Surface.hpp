#ifndef MAGMATIC_SURFACE_HPP
#define MAGMATIC_SURFACE_HPP

#include "PhysicalDevice.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic
{
	class Surface
	{
	public:
		friend class Instance;

		const vk::UniqueSurfaceKHR surface;

		vk::SurfaceCapabilitiesKHR getCapabilities(const PhysicalDevice& physical_device) const;
		std::vector<vk::SurfaceFormatKHR> getFormat(const PhysicalDevice& physical_device) const;

		Surface(const Surface&) = delete;
		Surface& operator=(const Surface&) = delete;

	private:
		explicit Surface(vk::UniqueSurfaceKHR surface);
	};
}

#endif //MAGMATIC_SURFACE_HPP

#ifndef MAGMATIC_SURFACE_HPP
#define MAGMATIC_SURFACE_HPP

#include "PhysicalDevice.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Surface
	{
		vk::UniqueSurfaceKHR surface;
	public:
		friend class Instance;

		[[nodiscard]] vk::SurfaceCapabilitiesKHR getCapabilities(const PhysicalDevice& physical_device) const;
		[[nodiscard]] std::vector<vk::SurfaceFormatKHR> getFormat(const PhysicalDevice& physical_device) const;

		Surface(const Surface&) = delete;
		Surface& operator=(const Surface&) = delete;

		Surface(Surface&& rhs) noexcept;
		Surface& operator=(Surface&& rhs) noexcept;

		[[nodiscard]] const vk::UniqueSurfaceKHR& getSurface() const {
			return this->surface;
		}

	private:
		explicit Surface(vk::UniqueSurfaceKHR surface);
	};
}

#endif //MAGMATIC_SURFACE_HPP

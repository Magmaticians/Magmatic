#ifndef MAGMATIC_SURFACE_HPP
#define MAGMATIC_SURFACE_HPP

#include "PhysicalDevice.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Surface
	{
	public:
		Surface(const Surface&) = delete;
		Surface& operator=(const Surface&) = delete;

		Surface(Surface&& rhs) noexcept = default;
		Surface& operator=(Surface&& rhs) noexcept = default;

		~Surface() = default;

		[[nodiscard]] vk::SurfaceCapabilitiesKHR getCapabilities(const PhysicalDevice& physical_device) const;
		[[nodiscard]] std::vector<vk::SurfaceFormatKHR> getFormat(const PhysicalDevice& physical_device) const;

		[[nodiscard]] const vk::UniqueSurfaceKHR& getSurface() const;

	private:
		vk::UniqueSurfaceKHR surface;
		friend class Instance;
		explicit Surface(vk::UniqueSurfaceKHR surface);
	};
}

#endif //MAGMATIC_SURFACE_HPP

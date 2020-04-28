#ifndef MAGMATIC_UTILS_HPP
#define MAGMATIC_UTILS_HPP

#include "PhysicalDevice.hpp"

namespace magmatic::render::utils {
	uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& propertyFlags, const PhysicalDevice& physicalDevice);

	vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
			const vk::FormatFeatureFlags& features, const PhysicalDevice& physicalDevice);

	static inline bool hasStencilComponent(vk::Format format) {
		return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
	}
}

#endif //MAGMATIC_UTILS_HPP

#ifndef MAGMATIC_UTILS_HPP
#define MAGMATIC_UTILS_HPP

#include "PhysicalDevice.hpp"

namespace magmatic::render::utils {
	uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& propertyFlags, const PhysicalDevice& physicalDevice);
}

#endif //MAGMATIC_UTILS_HPP

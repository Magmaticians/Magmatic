#include <vulkan/vulkan.hpp>
#include "utils.hpp"
#include <spdlog/spdlog.h>

uint32_t magmatic::utils::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& propertyFlags, const PhysicalDevice& physicalDevice) {
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.device.getMemoryProperties();
	for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
			return i;
		}
	}
	spdlog::error("magmatic::utils: Did not manage to find suitable memory type!");
	throw std::runtime_error("magmatic::utils: Did not manage to find suitable memory type!");
}

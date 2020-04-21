#include <vulkan/vulkan.hpp>
#include "render/utils.hpp"
#include <spdlog/spdlog.h>

uint32_t magmatic::render::utils::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags& propertyFlags, const PhysicalDevice& physicalDevice) {
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.device.getMemoryProperties();
	for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
			return i;
		}
	}
	spdlog::error("magmatic: Did not manage to find suitable memory type!");
	throw std::runtime_error("magmatic: Did not manage to find suitable memory type!");
}

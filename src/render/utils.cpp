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
	spdlog::error("magmatic-render: Failed to find suitable memory type!");
	throw std::runtime_error("magmatic: Failed to find suitable memory type!");
}

vk::Format magmatic::render::utils::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                               const vk::FormatFeatureFlags& features, const PhysicalDevice& physicalDevice) {
	for(auto format : candidates) {
		vk::FormatProperties props = physicalDevice.getFormatProperties(format);
		if(tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		} else if(tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	spdlog::error("magmatic-render: Failed to find supported format!");
	throw std::runtime_error("magmatic-render: Failed to find supported format!");
}

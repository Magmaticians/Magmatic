#ifndef MAGMATIC_BUFFER_HPP
#define MAGMATIC_BUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {
	class Buffer {
		friend class LogicalDevice;
	public:
		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;

		explicit Buffer(vk::UniqueBuffer buffer, vk::UniqueDeviceMemory vertexMemory) : buffer(std::move(buffer)), memory(std::move(vertexMemory)) {}

	public:
		Buffer(const Buffer&) = delete;
		Buffer& operator=(Buffer&) = delete;
	};
}

#endif //MAGMATIC_BUFFER_HPP

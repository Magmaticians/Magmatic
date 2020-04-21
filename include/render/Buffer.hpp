#ifndef MAGMATIC_BUFFER_HPP
#define MAGMATIC_BUFFER_HPP

#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Buffer {
		friend class LogicalDevice;
	public:
		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;

		explicit Buffer(vk::UniqueBuffer buffer, vk::UniqueDeviceMemory vertexMemory) : buffer(std::move(buffer)), memory(std::move(vertexMemory)) {}

	public:
		Buffer(const Buffer&) = delete;
		Buffer& operator=(Buffer&) = delete;

		Buffer(Buffer&& rhs) noexcept : buffer(std::move(rhs.buffer)), memory(std::move(rhs.memory)) {};
		Buffer& operator=(Buffer&& rhs) noexcept;
	};
}

#endif //MAGMATIC_BUFFER_HPP

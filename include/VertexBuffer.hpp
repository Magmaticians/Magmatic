#ifndef MAGMATIC_VERTEXBUFFER_HPP
#define MAGMATIC_VERTEXBUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {
	class VertexBuffer {
		friend class LogicalDevice;
	public:
		vk::UniqueBuffer vertexBuffer;
		vk::UniqueDeviceMemory vertexMemory;

		explicit VertexBuffer(vk::UniqueBuffer buffer, vk::UniqueDeviceMemory vertexMemory) : vertexBuffer(std::move(buffer)), vertexMemory(std::move(vertexMemory)) {}

	public:
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&) = delete;
	};
}

#endif //MAGMATIC_VERTEXBUFFER_HPP

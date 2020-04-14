#ifndef MAGMATIC_VERTEXBUFFER_HPP
#define MAGMATIC_VERTEXBUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {
	class VertexBuffer {
		friend class LogicalDevice;
	public:
		vk::UniqueBuffer vertexBuffer;

		explicit VertexBuffer(vk::UniqueBuffer buffer) : vertexBuffer(std::move(buffer)) {}

	public:
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&) = delete;
	};
}

#endif //MAGMATIC_VERTEXBUFFER_HPP

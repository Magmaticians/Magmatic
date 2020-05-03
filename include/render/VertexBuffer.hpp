#ifndef MAGMATIC_VERTEXBUFFER_HPP
#define MAGMATIC_VERTEXBUFFER_HPP

#include "render/LogicalDevice.hpp"
#include "render/Buffer.hpp"
#include "render/Vertex.hpp"
#include "render/CommandPool.hpp"
#include <vector>

namespace magmatic::render
{
	class VertexBuffer : public Buffer
	{
	private:
		size_t vertices_count;
		constexpr static vk::BufferUsageFlags buffer_usage_flags =
				vk::BufferUsageFlagBits::eTransferDst
				| vk::BufferUsageFlagBits::eVertexBuffer;

		constexpr static vk::MemoryPropertyFlags memory_property = vk::MemoryPropertyFlagBits::eDeviceLocal;

		constexpr static size_t vert_size = sizeof(Vertex);

	public:
		VertexBuffer(
				const LogicalDevice& l_device,
				const std::vector<Vertex>& vertices,
				const CommandPool& pool
				);

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& rhs) noexcept;
		VertexBuffer& operator=(VertexBuffer&& rhs) noexcept;

		[[nodiscard]] size_t verticesCount() const noexcept;

	};
}

#endif //MAGMATIC_VERTEXBUFFER_HPP

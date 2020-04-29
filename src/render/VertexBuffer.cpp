#include "render/VertexBuffer.hpp"

magmatic::render::VertexBuffer::VertexBuffer(
		const LogicalDevice& l_device,
		const std::vector<Vertex>& vertices,
		const CommandPool& pool
		)
:Buffer(l_device, vk::DeviceSize(vert_size*vertices.size()), buffer_usage_flags, memory_property),
vertices_count(vertices.size())
{
	const auto data_size = sizeof(Vertex) * vertices.size();

	const auto staging = Buffer::createStaging(
			l_device,
			vertices.data(),
			data_size
	);

	Buffer::copyBuffer(staging.getBuffer(), buffer, data_size, pool);
}

size_t magmatic::render::VertexBuffer::verticesCount() const noexcept
{
	return vertices_count;
}

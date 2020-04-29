#include "render/IndexBuffer.hpp"

magmatic::render::IndexBuffer::IndexBuffer(const magmatic::render::LogicalDevice& l_device,
                                           const std::vector<uint32_t>& indices,
                                           const magmatic::render::CommandPool& pool
)
: Buffer(l_device, vk::DeviceSize(sizeof(uint32_t) * indices.size()), buffer_usage_flags, memory_property),
  indices_count(indices.size())
{

	const auto data_size = sizeof(uint32_t) * indices.size();

	const auto staging = Buffer::createStaging(
			l_device,
			indices.data(),
			data_size
	);

	Buffer::copyBuffer(staging.getBuffer(), buffer, data_size, pool);
}

size_t magmatic::render::IndexBuffer::indicesCount() const noexcept
{
	return indices_count;
}



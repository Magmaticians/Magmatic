#include "render/CommandBuffer.hpp"
#include <spdlog/spdlog.h>

magmatic::render::CommandBuffer& magmatic::render::CommandBuffer::operator=(magmatic::render::CommandBuffer&& rhs) noexcept
{
	command_buffer = std::move(rhs.command_buffer);
	queue = rhs.queue;
	return *this;
}

const vk::UniqueCommandBuffer& magmatic::render::CommandBuffer::beginRecording(vk::CommandBufferUsageFlags usage)
{
	if(recording)
	{
		spdlog::error("Magmatic: Recording started on not ended command buffer");
		throw std::runtime_error("Recording started on not ended command buffer");
	}
	recording = true;
	vk::CommandBufferBeginInfo begin_info{
		usage
	};
	command_buffer->begin(begin_info);
	return command_buffer;
}

void magmatic::render::CommandBuffer::endRecording()
{
	if(!recording)
	{
		spdlog::error("Magmatic: Cannot end not started recording");
		throw std::runtime_error("Magmatic: Cannot end not started recording");
	}
	command_buffer->end();
	recording = false;
}

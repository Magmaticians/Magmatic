#include "CommandBuffer.hpp"
#include <spdlog/spdlog.h>

magmatic::CommandBuffer& magmatic::CommandBuffer::operator=(magmatic::CommandBuffer&& rhs) noexcept
{
	command_buffer = std::move(rhs.command_buffer);
	queue = rhs.queue;
	return *this;
}

const vk::UniqueCommandBuffer& magmatic::CommandBuffer::beginRecording(vk::CommandBufferUsageFlags usage)
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

void magmatic::CommandBuffer::endRecording()
{
	if(!recording)
	{
		spdlog::error("Magmatic: Cannot end not started recording");
		throw std::runtime_error("Magmatic: Cannot end not started recording");
	}
	command_buffer->end();
	recording = false;
}

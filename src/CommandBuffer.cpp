#include "CommandBuffer.hpp"


const vk::UniqueCommandBuffer& magmatic::CommandBuffer::beginRecording(vk::CommandBufferUsageFlags usage)
{
	recording_mutex.lock();
	vk::CommandBufferBeginInfo begin_info{
		usage
	};
	command_buffer->begin(begin_info);
	return command_buffer;
}

void magmatic::CommandBuffer::endRecording()
{
	command_buffer->end();
	recording_mutex.unlock();
}
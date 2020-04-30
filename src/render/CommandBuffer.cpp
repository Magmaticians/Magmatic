#define NOMINMAX
#include "render/CommandBuffer.hpp"
#include <spdlog/spdlog.h>
#include <limits>

magmatic::render::CommandBuffer& magmatic::render::CommandBuffer::operator=(magmatic::render::CommandBuffer&& rhs) noexcept
{
	command_buffer = std::move(rhs.command_buffer);
	queue = rhs.queue;
	return *this;
}

const vk::UniqueCommandBuffer& magmatic::render::CommandBuffer::beginRecording(const vk::CommandBufferUsageFlags& usage)
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

void magmatic::render::CommandBuffer::submitWait()
{
	auto logical_device = command_buffer.getOwner();
	auto fence = logical_device.createFenceUnique(vk::FenceCreateInfo());

	vk::SubmitInfo submit_info;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer.get();

	queue.submit(submit_info, fence.get());
	logical_device.waitForFences(1, &fence.get(), true, std::numeric_limits<uint64_t>::max());
}

magmatic::render::CommandBuffer::CommandBuffer(magmatic::render::CommandBuffer&& rhs) noexcept
: queue(rhs.queue), command_buffer(std::move(rhs.command_buffer)) {}

magmatic::render::CommandBuffer::CommandBuffer(const magmatic::render::CommandPool& pool, vk::CommandBufferLevel level)
:queue(pool.getQueue())
{
	vk::CommandBufferAllocateInfo command_buffer_info {
		pool.getHandle().get(),
		level,
		1
	};

	const auto& l_device = pool.getHandle().getOwner();
	command_buffer = std::move(l_device.allocateCommandBuffersUnique(command_buffer_info).front());
}

void magmatic::render::CommandBuffer::submit(const Semaphores& imageAcquiredSemaphores, const Semaphores& renderFinishedSemaphores, const vk::UniqueFence& fence, size_t index) const {
	vk::PipelineStageFlags flags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	vk::SubmitInfo submitInfo(
			1,
			&imageAcquiredSemaphores.semaphores[index].get(),
			&flags,
			1,
			&command_buffer.get(),
			1,
			&renderFinishedSemaphores.semaphores[index].get());
	queue.submit(submitInfo, fence.get());
}

std::vector<magmatic::render::CommandBuffer> magmatic::render::CommandBuffer::createCommandBuffers(size_t count, const CommandPool& pool, vk::CommandBufferLevel level) {
	std::vector<CommandBuffer> res;
	res.reserve(count);
	for(size_t i = 0; i < count; i++) {
		res.emplace_back(CommandBuffer(pool, level));
	}
	return res;
}

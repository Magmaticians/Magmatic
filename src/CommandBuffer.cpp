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


void magmatic::CommandBuffer::beginRenderPass(const RenderPass& renderPass, vk::ResultValue<uint32_t> currentBuffer, const Framebuffers& framebuffers, vk::Extent2D extent) {
	if(renderPassRunning) {
		spdlog::error("Magmatic: Render pass started on not ended render pass");
		throw std::runtime_error("Magmatic: Render pass started on not ended render pass");
	}

	vk::ClearValue clearValue;
	clearValue.color = vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 0.2f}));
	vk::RenderPassBeginInfo beginInfo(renderPass.renderPass.get(),
	                                  framebuffers.framebuffers[currentBuffer.value].get(),
	                                  vk::Rect2D(vk::Offset2D(0, 0), extent),
	                                  1,
	                                  &clearValue);
	command_buffer->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
	renderPassRunning = true;
}

void magmatic::CommandBuffer::endRenderPass() {
	if(!renderPassRunning) {
		spdlog::error("Magmatic: Render pass ended on ended render pass");
		throw std::runtime_error("Magmatic: Render pass ended on ended render pass");
	}
	command_buffer->endRenderPass();
	renderPassRunning = false;
}

void magmatic::CommandBuffer::bindPipeline(const Pipeline& pipeline) {
	command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline.get());
}

void magmatic::CommandBuffer::setViewport(vk::Extent2D extent) const {
	command_buffer->setViewport(0,
			vk::Viewport(
					0.0f,
					0.0f,
					static_cast<float>(extent.width),
					static_cast<float>(extent.height),
					0.0f,
					1.0f));
}

void magmatic::CommandBuffer::setScissor(vk::Extent2D extent) const {
	command_buffer->setScissor(0, vk::Rect2D(vk::Offset2D(0,0), extent));
}

void magmatic::CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const {
	command_buffer->draw(vertexCount, instanceCount, firstVertex, firstInstance);
}
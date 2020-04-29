#include <render/CommandBuffer.hpp>
#include "render/Buffer.hpp"
#include "render/utils.hpp"

magmatic::render::Buffer::Buffer(const LogicalDevice& l_device, vk::DeviceSize size,
                                 vk::BufferUsageFlags usageFlags, vk::MemoryPropertyFlags memoryFlags)
{
	const auto& handle = l_device.getHandle();
	uint32_t graphics_queue_index = l_device.getGraphicsQueueIndex();

	vk::BufferCreateInfo bufferCreateInfo(
			vk::BufferCreateFlags(),
			size,
			usageFlags,
			vk::SharingMode::eExclusive,
			1,
			&graphics_queue_index
	);
	buffer = handle->createBufferUnique(bufferCreateInfo);

	vk::MemoryRequirements memoryRequirements = l_device.getHandle()->getBufferMemoryRequirements(buffer.get());
	vk::MemoryAllocateInfo memoryAllocateInfo(
			memoryRequirements.size,
			magmatic::render::utils::findMemoryType(
					memoryRequirements.memoryTypeBits,
					memoryFlags, l_device.getPhysicalDevice()
					)
	);
	memory =  handle->allocateMemoryUnique(memoryAllocateInfo);
	handle->bindBufferMemory(buffer.get(), memory.get(), 0);
}


magmatic::render::Buffer::Buffer(magmatic::render::Buffer&& rhs) noexcept : buffer(std::move(rhs.buffer)), memory(std::move(rhs.memory)) {}

magmatic::render::Buffer& magmatic::render::Buffer::operator=(magmatic::render::Buffer&& rhs) noexcept {
	buffer = std::move(rhs.buffer);
	memory = std::move(rhs.memory);
	return *this;
}

const vk::UniqueBuffer& magmatic::render::Buffer::getBuffer() const noexcept
{
	return buffer;
}

const vk::UniqueDeviceMemory& magmatic::render::Buffer::getMemory() const noexcept
{
	return memory;
}

void magmatic::render::Buffer::copyBuffer(
		const vk::UniqueBuffer& src,
		const vk::UniqueBuffer& dst,
		vk::DeviceSize size,
		const CommandPool& pool
)
{
	CommandBuffer commandBuffer(pool);
	auto& commandBufferHandle = commandBuffer.beginRecording();
	vk::BufferCopy copyRegion(
			0,
			0,
			size
	);
	commandBufferHandle->copyBuffer(src.get(), dst.get(), 1, &copyRegion);
	commandBuffer.endRecording();

	commandBuffer.submitWait();
}

void magmatic::render::Buffer::copyBufferToImage(
		const vk::UniqueBuffer& src,
		const vk::UniqueImage& dst,
		uint32_t width,uint32_t height,
		const magmatic::render::CommandPool& pool
)
{
	CommandBuffer command_buffer(pool);
	const auto& cmd = command_buffer.beginRecording();

	vk::BufferImageCopy region{
			0,
			0,
			0,
			{
					vk::ImageAspectFlagBits::eColor,
					0,
					0,
					1
			},
			{0, 0, 0},
			{
					width,
					height,
					1
			}
	};

	cmd->copyBufferToImage(
			src.get(),
			dst.get(),
			vk::ImageLayout::eTransferDstOptimal,
			1,
			&region
	);

	command_buffer.endRecording();
	command_buffer.submitWait();
}




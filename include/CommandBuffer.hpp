#ifndef MAGMATIC_COMMANDBUFFER_HPP
#define MAGMATIC_COMMANDBUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <atomic>
#include <mutex>

namespace magmatic
{
	class CommandBuffer
	{
	public:
		friend class LogicalDevice;

		CommandBuffer(const CommandBuffer&) = delete;
		CommandBuffer& operator=(CommandBuffer&) = delete;

		CommandBuffer(CommandBuffer&& rhs) noexcept : queue(rhs.queue), command_buffer(std::move(rhs.command_buffer)) {};
		CommandBuffer& operator=(CommandBuffer&& rhs) noexcept;

		const vk::UniqueCommandBuffer& beginRecording(vk::CommandBufferUsageFlags usage = vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		void endRecording();

	private:

		CommandBuffer(vk::UniqueCommandBuffer& buffer,  vk::Queue queue) noexcept
		: command_buffer(std::move(buffer)), queue(queue){};

		vk::Queue queue;
		vk::UniqueCommandBuffer command_buffer;

		bool recording;
	};
}


#endif //MAGMATIC_COMMANDBUFFER_HPP

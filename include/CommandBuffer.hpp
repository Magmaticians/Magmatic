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

		const vk::UniqueCommandBuffer& beginRecording(vk::CommandBufferUsageFlags usage = vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		void endRecording();

	private:

		explicit CommandBuffer(vk::UniqueCommandBuffer buffer,const vk::Queue& queue)
		: command_buffer(std::move(buffer)), queue(queue){};

		const vk::Queue& queue;
		const vk::UniqueCommandBuffer command_buffer;

		std::mutex recording_mutex;
	};
}


#endif //MAGMATIC_COMMANDBUFFER_HPP

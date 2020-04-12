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

		explicit CommandBuffer(vk::UniqueCommandBuffer buffer, vk::Queue queue)
		: command_buffer(std::move(buffer)), queue(queue){};

		const std::reference_wrapper<vk::Queue> queue;
		const vk::UniqueCommandBuffer command_buffer;

		bool recording;
	};
}


#endif //MAGMATIC_COMMANDBUFFER_HPP

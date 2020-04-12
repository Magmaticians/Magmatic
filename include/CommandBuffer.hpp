#ifndef MAGMATIC_COMMANDBUFFER_HPP
#define MAGMATIC_COMMANDBUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic
{
	class CommandBuffer
	{
	public:
		friend class LogicalDevice;
		const vk::UniqueCommandBuffer command_buffer;
		const vk::Queue& queue;

		CommandBuffer(const CommandBuffer&) = delete;
		CommandBuffer& operator=(CommandBuffer&) = delete;

	private:
		explicit CommandBuffer(vk::UniqueCommandBuffer buffer,const vk::Queue& queue)
		: command_buffer(std::move(buffer)), queue(queue){};

	};
}


#endif //MAGMATIC_COMMANDBUFFER_HPP

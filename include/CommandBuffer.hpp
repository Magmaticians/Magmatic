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

		CommandBuffer(const CommandBuffer&) = delete;
		CommandBuffer& operator=(CommandBuffer&) = delete;

	private:
		explicit CommandBuffer(vk::UniqueCommandBuffer buffer) : command_buffer(std::move(buffer)){};

	};
}


#endif //MAGMATIC_COMMANDBUFFER_HPP

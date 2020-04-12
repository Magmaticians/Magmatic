#ifndef MAGMATIC_COMMANDPOOL_HPP
#define MAGMATIC_COMMANDPOOL_HPP

#include <vulkan/vulkan.hpp>
#include "QueueType.hpp"


namespace magmatic
{
	class CommandPool
	{
		friend class LogicalDevice;
		const vk::UniqueCommandPool command_pool;
		const QueueType type;

		explicit CommandPool(vk::UniqueCommandPool pool, QueueType type) : command_pool(std::move(pool)), type(type){};

	public:
		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(CommandPool&) = delete;
	};
}


#endif //MAGMATIC_COMMANDPOOL_HPP

#ifndef MAGMATIC_COMMANDPOOL_HPP
#define MAGMATIC_COMMANDPOOL_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic
{
	class CommandPool
	{
		friend class LogicalDevice;
		const vk::UniqueCommandPool command_pool;

		explicit CommandPool(vk::UniqueCommandPool pool) : command_pool(std::move(pool)) {}

	public:
		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(CommandPool&) = delete;
	};
}


#endif //MAGMATIC_COMMANDPOOL_HPP

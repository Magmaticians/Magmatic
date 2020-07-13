#ifndef MAGMATIC_COMMANDPOOL_HPP
#define MAGMATIC_COMMANDPOOL_HPP

#include <vulkan/vulkan.hpp>
#include "QueueType.hpp"
#include "render/LogicalDevice.hpp"

namespace magmatic::render
{
	class CommandPool
	{
	private:
		vk::UniqueCommandPool command_pool;
		QueueType type;
		vk::Queue queue;

	public:
		CommandPool(const LogicalDevice& l_device, QueueType type);

		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(const CommandPool&) = delete;

		CommandPool(CommandPool&& rhs) noexcept = default;
		CommandPool& operator=(CommandPool&& rhs) noexcept = default;

		~CommandPool() = default;

		[[nodiscard]] const vk::UniqueCommandPool& getHandle() const noexcept;
		[[nodiscard]] const vk::Queue& getQueue() const noexcept;
	};
}


#endif //MAGMATIC_COMMANDPOOL_HPP

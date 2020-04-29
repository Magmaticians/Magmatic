#include "render/CommandPool.hpp"

magmatic::render::CommandPool::CommandPool(const LogicalDevice& l_device, QueueType type): type(type) {
	const auto& handle = l_device.getHandle();
	size_t queue_family_index;
	switch(type)
	{
		case QueueType::GraphicalQueue:
			queue_family_index = l_device.getGraphicsQueueIndex();
			queue = l_device.getGraphicsQueue();
			break;
		case QueueType::PresentQueue:
			queue_family_index = l_device.getPresentQueueIndex();
			queue = l_device.getPresentQueue();
			break;
	}
	command_pool = handle->createCommandPoolUnique(
			vk::CommandPoolCreateInfo(
					vk::CommandPoolCreateFlags(),
					queue_family_index
			));
}

const vk::UniqueCommandPool& magmatic::render::CommandPool::getHandle() const noexcept
{
	return command_pool;
}

const vk::Queue& magmatic::render::CommandPool::getQueue() const noexcept
{
	return queue;
}

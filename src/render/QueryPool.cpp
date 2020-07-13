#include "render/QueryPool.hpp"

magmatic::render::QueryPool::QueryPool(const LogicalDevice logical_device,
		uint32_t count,
		vk::QueryType type,
		const vk::QueryPipelineStatisticFlagBits& statistic_flag_bits
		)
: query_pool_(createPool(logical_device, count, type, statistic_flag_bits))
{
}

vk::UniqueQueryPool magmatic::render::QueryPool::createPool(
		const magmatic::render::LogicalDevice logical_device, uint32_t count, vk::QueryType type
		, const vk::QueryPipelineStatisticFlagBits& statistic_flag_bits
)
{
	const vk::QueryPoolCreateInfo create_info
			{
					{},
					type,
					count,
					statistic_flag_bits,
			};

	return logical_device.getHandle()->createQueryPoolUnique(create_info);}

const vk::UniqueQueryPool magmatic::render::QueryPool::handle() noexcept
{
	return query_pool_;
}

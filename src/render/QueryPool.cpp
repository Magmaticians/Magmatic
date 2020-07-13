#include "render/QueryPool.hpp"

magmatic::render::QueryPool::QueryPool(const LogicalDevice& logical_device,
		uint32_t count,
		vk::QueryType type,
		const vk::QueryPipelineStatisticFlagBits& statistic_flag_bits
		)
: query_pool_(createPool(logical_device, count, type, statistic_flag_bits))
{
}

vk::UniqueQueryPool magmatic::render::QueryPool::createPool(
		const magmatic::render::LogicalDevice& logical_device, uint32_t count, vk::QueryType type
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

const vk::UniqueQueryPool& magmatic::render::QueryPool::handle() const noexcept
{
	return query_pool_;
}

std::vector<uint64_t> magmatic::render::QueryPool::getResults(
		std::size_t count, std::size_t first, bool wait
		) noexcept
{
	std::vector<uint64_t > results (count);

	vk::QueryResultFlags flags = vk::QueryResultFlagBits::e64;
	if(wait)
	{
		flags |= vk::QueryResultFlagBits::eWait;
	}

	query_pool_.getOwner().getQueryPoolResults
	(
		*query_pool_,
		first, count,
		vk::ArrayProxy<uint64_t>(results),
		sizeof(decltype(results)::value_type),
		flags
	);

	return results;
}

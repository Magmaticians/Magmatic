#ifndef MAGMATIC_QUERYPOOL_HPP
#define MAGMATIC_QUERYPOOL_HPP

#include "LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>

namespace magmatic::render
{
	class QueryPool
	{
	public:
		QueryPool(
				const LogicalDevice& logical_device, uint32_t count, vk::QueryType type = vk::QueryType::eOcclusion
				, const vk::QueryPipelineStatisticFlagBits& statistic_flag_bits = {}
		);

		QueryPool(const QueryPool&) = delete;
		QueryPool& operator=(const QueryPool&) = delete;

		QueryPool(QueryPool&&) = default;
		QueryPool& operator=(QueryPool&&) = default;

		~QueryPool() = default;

		std::vector<uint64_t> getResults(std::size_t count, std::size_t first, bool wait) noexcept;

		const vk::UniqueQueryPool& handle() const noexcept;
	private:
		vk::UniqueQueryPool query_pool_;

		static vk::UniqueQueryPool createPool(
				const LogicalDevice& logical_device,
				uint32_t count, vk::QueryType type, const vk::QueryPipelineStatisticFlagBits& statistic_flag_bits
				);
	};
}

#endif //MAGMATIC_QUERYPOOL_HPP

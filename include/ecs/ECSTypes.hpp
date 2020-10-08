#ifndef MAGMATIC_ECSTYPES_HPP
#define MAGMATIC_ECSTYPES_HPP

#include "utils/thread_pool/ThreadPool.hpp"
#include "ECSConfig.hpp"
#include <bitset>

namespace magmatic::ecs
{
	using thread_pool_t = magmatic::utils::ThreadPool;
	using thread_pool_ptr = std::shared_ptr<thread_pool_t>;

	using delta_time_t = std::chrono::duration<int64_t, std::micro>;

	using entity_id_t = std::size_t;
	using components_mask_t = std::bitset<COMPONENT_TYPE_COUNT>;

}

#endif //MAGMATIC_ECSTYPES_HPP

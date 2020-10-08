#include "ecs/executor/ParallelExecutor.hpp"


magmatic::ecs::executor::ParallelExecutor::ParallelExecutor
(
		thread_pool_ptr thread_pool, ECS& ecs
) noexcept
:parameters_{std::move(thread_pool), ecs, {}}
{
}

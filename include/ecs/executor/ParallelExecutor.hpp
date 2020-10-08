#ifndef MAGMATIC_PARALLELEXECUTOR_HPP
#define MAGMATIC_PARALLELEXECUTOR_HPP

#include <vector>
#include <memory>
#include "utils/dgraph/DGraph.hpp"
#include "utils/thread_pool/ThreadPool.hpp"
#include "../ECSTypes.hpp"

namespace magmatic::ecs
{
	class ECS;
}

namespace magmatic::ecs::executor
{
	class ParallelExecutor
	{
	public:
		using ecs_t = magmatic::ecs::ECS;

		ParallelExecutor(thread_pool_ptr thread_pool, ecs_t& ecs) noexcept;

	private:
		struct SharedTaskParameters
		{
			thread_pool_ptr thread_pool;
			ecs_t& ecs;
			delta_time_t current_delta;
		};

	class Task : public std::enable_shared_from_this<Task>
		{
		public:
			Task(
					std::function<void(void)> task,
					SharedTaskParameters& shared_task_parameters,
					std::vector<std::shared_ptr<Task>> dependant_tasks,
					uint16_t dependency_count=0
					)
			:
			task_(std::move(task)),
			parameters_(shared_task_parameters),
			dependant_tasks_(std::move(dependant_tasks)),
			dependency_count_(dependency_count)
			{}

			void joinDependency()
			{
				const auto current_dependency_count = --dependency_count_;
				assert(current_dependency_count);
				
				if(current_dependency_count == 0)
				{
					execute();
				}
			}

		private:

			void execute()
			{
				parameters_.thread_pool->enqueue([task=shared_from_this()](){
					task->task_();
					task->notifyDependants();
				});
			}

			void notifyDependants()
			{
				for(const auto& task : dependant_tasks_)
				{
					task->joinDependency();
				}
			}

			std::function<void(void)> task_;
			SharedTaskParameters& parameters_;
			std::vector<std::shared_ptr<Task>> dependant_tasks_;
			std::atomic_uint16_t dependency_count_;
		};

		using task_ptr = std::shared_ptr<Task>;

		DGraph<int> dependency_graph_;
		SharedTaskParameters parameters_;
		std::vector <task_ptr> independent_tasks_;


	};
}


#endif //MAGMATIC_PARALLELEXECUTOR_HPP

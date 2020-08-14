#include "utils/thread_pool/ThreadPool.hpp"
#include <cassert>

magmatic::utils::ThreadPool::ThreadPool(std::size_t thread_count)
{
	assert(thread_count != 0);

	for(std::size_t i = 0; i < thread_count; ++i)
	{
		workers_.emplace_back(std::thread(
			[&]()
			{
				std::unique_lock queue_lock(queue_mutex_, std::defer_lock);
				while(true)
				{
					queue_lock.lock();
					tasks_cv_.wait(
							queue_lock,
							[&]()
							{
								return !tasks_.empty() || quit_;
							}
					);

					if(quit_)
					{
						return;
					}

					auto work = std::move(tasks_.front());
					tasks_.pop();

					queue_lock.unlock();

					work();
				}
			}
		));
	}
}

magmatic::utils::ThreadPool::~ThreadPool()
{
	std::unique_lock queue_lock(queue_mutex_);
	quit_ = true;
	queue_lock.unlock();

	tasks_cv_.notify_all();
	for(auto& worker: workers_)
	{
		//can be done with killing using system specific way
		worker.join();
	}
}

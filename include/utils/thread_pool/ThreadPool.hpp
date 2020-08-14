#ifndef MAGMATIC_THREADPOOL_HPP
#define MAGMATIC_THREADPOOL_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>


namespace magmatic::utils
{
	class ThreadPool
	{
	public:
		explicit ThreadPool(std::size_t thread_count=std::thread::hardware_concurrency());
		~ThreadPool();

		template<typename F, typename... Args>
		requires std::invocable<F, Args...>
		auto enqueue(F fun, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

		[[nodiscard]] std::size_t thread_count() const
		{
			return workers_.size();
		}
	private:
		std::vector<std::thread> workers_;

		std::mutex queue_mutex_;
		std::condition_variable tasks_cv_;

		std::queue<std::function<void(void)>> tasks_; // todo: change to non-copyable function container to prevent usage of shared ptr

		bool quit_ = false;
	};

	template<typename F, typename... Args>
	requires std::invocable<F, Args...>
	auto ThreadPool::enqueue(F fun, Args&& ... args) -> std::future<std::invoke_result_t<F, Args...>>
	{
		auto task = std::make_shared<std::packaged_task<std::invoke_result_t<F, Args...>()>>
		(
			[f = std::move(fun), ...f_args = std::forward<Args>(args)]()
			{
				return f(std::forward<Args>(f_args)...);
			}
		);

		std::future<std::invoke_result_t<F, Args...>> task_future = task->get_future();

		std::unique_lock queue_lock(queue_mutex_);

		tasks_.push([task]() {(*task)();});

		queue_lock.unlock();
		tasks_cv_.notify_one();

		return task_future;
	}

}

#endif //MAGMATIC_THREADPOOL_HPP

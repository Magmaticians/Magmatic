#include <gtest/gtest.h>
#include <utils/thread_pool/ThreadPool.hpp>


TEST(ThreadPoolTest, enqueue_tasks_default_size)
{
	magmatic::utils::ThreadPool thread_pool;

	auto task_1 = thread_pool.enqueue([](){return 7;});
	auto task_2 = thread_pool.enqueue([](){return 8;});

	ASSERT_EQ(7, task_1.get());
	ASSERT_EQ(8, task_2.get());
}

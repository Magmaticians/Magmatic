#ifndef MAGMATIC_SEMAPHORE_HPP
#define MAGMATIC_SEMAPHORE_HPP

#include <vulkan/vulkan.hpp>
#include "SemaphoreType.hpp"

namespace magmatic {
	class Semaphore {
	public:
		const vk::UniqueSemaphore semaphore;
		const SemaphoreType type;

		explicit Semaphore(vk::UniqueSemaphore semaphore, SemaphoreType type): semaphore(std::move(semaphore)), type(type) { }

		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;
	};
}

#endif //MAGMATIC_SEMAPHORE_HPP

#ifndef MAGMATIC_SEMAPHORES_HPP
#define MAGMATIC_SEMAPHORES_HPP

#include <vulkan/vulkan.hpp>
#include "SemaphoreType.hpp"

namespace magmatic::render {
	class Semaphores {
	public:
		const std::vector<vk::UniqueSemaphore> semaphores;
		const SemaphoreType type;

		explicit Semaphores(std::vector<vk::UniqueSemaphore> semaphores, SemaphoreType type): semaphores(std::move(semaphores)), type(type) { }

		Semaphores(const Semaphores&) = delete;
		Semaphores& operator=(const Semaphores&) = delete;
	};
}

#endif //MAGMATIC_SEMAPHORES_HPP

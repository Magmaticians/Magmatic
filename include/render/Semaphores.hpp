#ifndef MAGMATIC_SEMAPHORES_HPP
#define MAGMATIC_SEMAPHORES_HPP

#include <vulkan/vulkan.hpp>
#include "SemaphoreType.hpp"
#include "LogicalDevice.hpp"

namespace magmatic::render {
	class Semaphores {
	public:
		std::vector<vk::UniqueSemaphore> semaphores;
		SemaphoreType type;

		Semaphores(const LogicalDevice& l_device, SemaphoreType type, size_t count);

		Semaphores(const Semaphores&) = delete;
		Semaphores& operator=(const Semaphores&) = delete;

		Semaphores(Semaphores&& rhs) noexcept;
		Semaphores& operator=(Semaphores&& rhs) noexcept;
	};
}

#endif //MAGMATIC_SEMAPHORES_HPP

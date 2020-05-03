#ifndef MAGMATIC_SEMAPHORES_HPP
#define MAGMATIC_SEMAPHORES_HPP

#include <vulkan/vulkan.hpp>
#include "SemaphoreType.hpp"
#include "LogicalDevice.hpp"

namespace magmatic::render {
	class Semaphores {
		std::vector<vk::UniqueSemaphore> semaphores;
	public:
		SemaphoreType type;

		Semaphores(const LogicalDevice& l_device, SemaphoreType type, size_t count);

		Semaphores(const Semaphores&) = delete;
		Semaphores& operator=(const Semaphores&) = delete;

		Semaphores(Semaphores&& rhs) noexcept;
		Semaphores& operator=(Semaphores&& rhs) noexcept;

		[[nodiscard]] const std::vector<vk::UniqueSemaphore>& getSemaphores() const {
			return this->semaphores;
		}

		[[nodiscard]] const vk::UniqueSemaphore& operator[](size_t index) const{
			return this->semaphores[index];
		}
	};
}

#endif //MAGMATIC_SEMAPHORES_HPP

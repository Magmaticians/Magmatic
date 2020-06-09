#ifndef MAGMATIC_SEMAPHORES_HPP
#define MAGMATIC_SEMAPHORES_HPP

#include <vulkan/vulkan.hpp>
#include "SemaphoreType.hpp"
#include "LogicalDevice.hpp"

namespace magmatic::render {
	class Semaphores {
	public:
		Semaphores(const LogicalDevice& l_device, SemaphoreType type, size_t count);

		Semaphores(const Semaphores&) = delete;
		Semaphores& operator=(const Semaphores&) = delete;

		Semaphores(Semaphores&& rhs) noexcept = default;
		Semaphores& operator=(Semaphores&& rhs) noexcept = default;

		~Semaphores() = default;

		[[nodiscard]] const std::vector<vk::UniqueSemaphore>& getSemaphores() const;

		[[nodiscard]] const vk::UniqueSemaphore& operator[](size_t index) const;

	private:
		std::vector<vk::UniqueSemaphore> semaphores;
		SemaphoreType type;

	};
}

#endif //MAGMATIC_SEMAPHORES_HPP

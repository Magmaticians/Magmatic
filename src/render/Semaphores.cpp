#include "render/Semaphores.hpp"

magmatic::render::Semaphores::Semaphores(const LogicalDevice& l_device, SemaphoreType type, size_t count): type(type) {
	const auto& handle = l_device.getHandle();

	semaphores.reserve(count);
	for(size_t i = 0; i < count; i++) {
		semaphores.emplace_back(handle->createSemaphoreUnique(vk::SemaphoreCreateInfo()));
	}
}

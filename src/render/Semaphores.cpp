#include "render/Semaphores.hpp"

magmatic::render::Semaphores::Semaphores(const LogicalDevice& l_device, SemaphoreType type, size_t count): type(type) {
	const auto& handle = l_device.getHandle();

	semaphores.reserve(count);
	for(size_t i = 0; i < count; i++) {
		semaphores.emplace_back(handle->createSemaphoreUnique(vk::SemaphoreCreateInfo()));
	}
}

const vk::UniqueSemaphore &magmatic::render::Semaphores::operator[](size_t index) const
{
	return this->semaphores[index];
}

const std::vector<vk::UniqueSemaphore> &magmatic::render::Semaphores::getSemaphores() const
{
	return this->semaphores;
}

#include "render/Fences.hpp"

magmatic::render::Fences::Fences(const LogicalDevice& l_device, size_t count) {
	const auto& handle = l_device.getHandle();
	fences.reserve(count);
	for(size_t i = 0; i < count; i++) {
		fences.emplace_back(handle->createFenceUnique(vk::FenceCreateInfo()));
	}
}

void magmatic::render::Fences::waitForFence(size_t index, uint64_t timeout) const {
	const auto& handle = fences[index].getOwner();
	handle.waitForFences(fences[index].get(), VK_TRUE, timeout);
}

void magmatic::render::Fences::resetFence(size_t index) const {
	const auto& handle = fences[index].getOwner();
	handle.resetFences(1, &fences[index].get());
}

magmatic::render::Fences::Fences(magmatic::render::Fences &&rhs) noexcept : fences(std::move(rhs.fences)) { }

magmatic::render::Fences &magmatic::render::Fences::operator=(magmatic::render::Fences &&rhs) noexcept {
	this->fences = std::move(rhs.fences);
	return *this;
}

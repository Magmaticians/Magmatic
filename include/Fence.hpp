#ifndef MAGMATIC_FENCE_HPP
#define MAGMATIC_FENCE_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {
	class Fence {
	public:
		vk::UniqueFence fence;

		explicit Fence(vk::UniqueFence fence) : fence(std::move(fence)) {}

		Fence(Fence&) = delete;
		Fence& operator=(Fence&) = delete;
	};
}

#endif //MAGMATIC_FENCE_HPP

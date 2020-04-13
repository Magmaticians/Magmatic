#ifndef MAGMATIC_FENCES_HPP
#define MAGMATIC_FENCES_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {
	class Fences {
	public:
		std::vector<vk::UniqueFence> fences;

		explicit Fences(std::vector<vk::UniqueFence> fences) : fences(std::move(fences)) {}

		Fences(Fences&) = delete;
		Fences& operator=(Fences&) = delete;

		void setFence(vk::UniqueFence fence, size_t index) {
			fences[index] = std::move(fence);
		}
	};
}

#endif //MAGMATIC_FENCES_HPP

#ifndef MAGMATIC_FENCES_HPP
#define MAGMATIC_FENCES_HPP

#include <vulkan/vulkan.hpp>
#include "LogicalDevice.hpp"

namespace magmatic::render
{
	class Fences
	{
		std::vector<vk::UniqueFence> fences;

	public:
		Fences(const LogicalDevice& l_device, size_t count);

		Fences(const Fences&) = delete;
		Fences& operator=(Fences&) = delete;

		void waitForFence(size_t index, uint64_t timeout) const;
		void resetFence(size_t index) const;

		const vk::UniqueFence& operator[](size_t index) const {
			return fences.at(index);
		}
	};
}

#endif //MAGMATIC_FENCES_HPP

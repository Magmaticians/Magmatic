#ifndef MAGMATIC_FENCES_HPP
#define MAGMATIC_FENCES_HPP

#include <vulkan/vulkan.hpp>
#include "LogicalDevice.hpp"

namespace magmatic::render
{
	class Fences
	{
	public:

		Fences(const LogicalDevice& l_device, size_t count);

		Fences(const Fences&) = delete;
		Fences& operator=(const Fences&) = delete;

		Fences(Fences&& rhs) noexcept = default;
		Fences& operator=(Fences&& rhs) noexcept = default;

		void waitForFence(size_t index, uint64_t timeout) const ;

		void resetFence(size_t index) const;
		const vk::UniqueFence& operator[](size_t index) const {
			return fences.at(index);
		}

	private:
		std::vector<vk::UniqueFence> fences;
	};
}

#endif //MAGMATIC_FENCES_HPP

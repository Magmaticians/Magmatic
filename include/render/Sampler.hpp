#ifndef MAGMATIC_SAMPLER_HPP
#define MAGMATIC_SAMPLER_HPP

#include <vulkan/vulkan.hpp>
#include "DescriptorWriteUpdate.hpp"


namespace magmatic::render
{
	class Sampler
	{
		friend class LogicalDevice;
	public:
		const vk::UniqueSampler sampler;

		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const
		{
			DescriptorWriteUpdate update;
			update.type = DescriptorWriteUpdate::eSampler;
			update.dst_binding = dst_binding;
			update.dst_array_elem = dst_array_elem;
			update.data_info = vk::DescriptorImageInfo {
				sampler.get()
			};
			return update;
		}

	private:
		explicit Sampler(vk::UniqueSampler sampler) : sampler(std::move(sampler)) {};
	};
}

#endif //MAGMATIC_SAMPLER_HPP

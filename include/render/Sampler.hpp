#ifndef MAGMATIC_SAMPLER_HPP
#define MAGMATIC_SAMPLER_HPP

#include "render/DescriptorWriteUpdate.hpp"
#include "render/LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Sampler
	{
	private:
		vk::UniqueSampler sampler;
	public:
		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const;

		explicit Sampler(const LogicalDevice& l_device,
		                 vk::Filter filter = vk::Filter::eNearest,
		                 float anisotropy_samples = 1.0f,
		                 bool normalized_coordinates = true
		);
	};
}

#endif //MAGMATIC_SAMPLER_HPP

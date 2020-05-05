#include "render/Sampler.hpp"

magmatic::render::Sampler::Sampler(const magmatic::render::LogicalDevice& l_device, vk::Filter filter,
                                   float anisotropy_samples, bool normalized_coordinates)
{
	vk::SamplerCreateInfo sampler_create_info = {
			vk::SamplerCreateFlags(),
			filter,
			filter,
			vk::SamplerMipmapMode::eNearest,
			vk::SamplerAddressMode::eClampToEdge,
			vk::SamplerAddressMode::eClampToEdge,
			vk::SamplerAddressMode::eClampToEdge,
			0.0f,
			anisotropy_samples > 1.0f,
			anisotropy_samples,
			false,
			vk::CompareOp::eAlways,
			0.0f,
			0.0f,
			vk::BorderColor::eIntOpaqueWhite,
			!normalized_coordinates
	};
	sampler = l_device.getHandle()->createSamplerUnique(sampler_create_info);
}

magmatic::render::DescriptorWriteUpdate
magmatic::render::Sampler::getWriteInfo(size_t dst_binding, size_t dst_array_elem) const {
	DescriptorWriteUpdate update;
	update.type = DescriptorWriteUpdate::eSampler;
	update.dst_binding = dst_binding;
	update.dst_array_elem = dst_array_elem;
	update.data_info = std::vector<vk::DescriptorImageInfo>{ {
			sampler.get()
	}};
	return update;
}

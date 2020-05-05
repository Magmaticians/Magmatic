#include <spdlog/spdlog.h>
#include "render/DescriptorWriteUpdate.hpp"


vk::WriteDescriptorSet magmatic::render::DescriptorWriteUpdate::toWriteInfo(
		const vk::DescriptorSet& descriptor
		) const
{
	vk::WriteDescriptorSet write_info;
	write_info.dstSet = descriptor;
	write_info.dstBinding = dst_binding;
	write_info.dstArrayElement = dst_array_elem;
	switch(type)
	{
		case eUniform:
			write_info.descriptorType = vk::DescriptorType::eUniformBuffer;
			write_info.pBufferInfo = std::get<std::vector<vk::DescriptorBufferInfo>>(data_info).data();
			write_info.descriptorCount = std::get<std::vector<vk::DescriptorBufferInfo>>(data_info).size();
			break;
		case eSampler:
			write_info.descriptorType = vk::DescriptorType::eSampler;
			write_info.pImageInfo = std::get<std::vector<vk::DescriptorImageInfo>>(data_info).data();
			write_info.descriptorCount = std::get<std::vector<vk::DescriptorImageInfo>>(data_info).size();
			break;
		case eImage:
			write_info.descriptorType = vk::DescriptorType::eSampledImage;
			write_info.pImageInfo = std::get<std::vector<vk::DescriptorImageInfo>>(data_info).data();
			write_info.descriptorCount = std::get<std::vector<vk::DescriptorImageInfo>>(data_info).size();
			break;
		default:
			spdlog::error("Magmatic: Descriptor set type not implemented");
			throw std::invalid_argument("Descriptor set type not implemented");
	}
	return write_info;
}


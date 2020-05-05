#ifndef MAGMATIC_DESCRIPTORWRITEUPDATE_HPP
#define MAGMATIC_DESCRIPTORWRITEUPDATE_HPP

#include <vulkan/vulkan.hpp>
#include <variant>


namespace magmatic::render
{
	struct DescriptorWriteUpdate
	{
		enum UpdateType
		{
			eUniform = 0,
			eImage = 1,
			eSampler = 2

		};
		UpdateType type;

		uint32_t dst_binding;
		uint32_t dst_array_elem;

		std::variant<std::vector<vk::DescriptorBufferInfo>, std::vector<vk::DescriptorImageInfo>> data_info;

		vk::WriteDescriptorSet toWriteInfo(const vk::DescriptorSet& descriptor) const;
	};
}


#endif //MAGMATIC_DESCRIPTORWRITEUPDATE_HPP

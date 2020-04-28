#ifndef MAGMATIC_TEXTURE_HPP
#define MAGMATIC_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include "DescriptorWriteUpdate.hpp"


namespace magmatic::render
{
	class Texture
	{
	public:
		friend class LogicalDevice;
		const vk::UniqueImage image;
		const vk::UniqueImageView image_view;
		const vk::UniqueDeviceMemory image_memory;

		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const
		{
			DescriptorWriteUpdate update;
			update.type = DescriptorWriteUpdate::eImage;
			update.dst_binding = dst_array_elem;
			update.dst_array_elem = dst_array_elem;
			update.data_info = vk::DescriptorImageInfo {
					{},
					image_view.get(),
					vk::ImageLayout::eShaderReadOnlyOptimal
			};
			return update;
		}

	private:
		Texture(vk::UniqueImage image, vk::UniqueImageView view, vk::UniqueDeviceMemory memory)
		: image(std::move(image)), image_view(std::move(view)), image_memory(std::move(memory)) {};
	};
}


#endif //MAGMATIC_TEXTURE_HPP

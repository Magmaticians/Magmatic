#ifndef MAGMATIC_TEXTURE_HPP
#define MAGMATIC_TEXTURE_HPP

#include "render/Image.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"
#include "render/DescriptorWriteUpdate.hpp"
#include "CommandPool.hpp"
#include "Bitmap.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Texture
	{
	public:
		Image image;
		vk::UniqueImageView image_view;

		Texture(const LogicalDevice& l_device, const Bitmap& bitmap, const CommandPool& pool);

		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const;
	private:
		[[nodiscard]] Image createImage(const LogicalDevice& l_device, const Bitmap& bitmap);
	};
}


#endif //MAGMATIC_TEXTURE_HPP

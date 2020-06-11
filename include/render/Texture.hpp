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
		Texture(const LogicalDevice& l_device, const Bitmap& bitmap, const CommandPool& pool);

		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const;

		const Image &getImage() const;

		const vk::UniqueImageView &getImageView() const;

	private:
		Image image;
		vk::UniqueImageView image_view;

		[[nodiscard]] static Image createImage(const LogicalDevice& l_device, const Bitmap& bitmap);
		[[nodiscard]] static vk::UniqueImageView createImageView(
				const LogicalDevice& l_device,
				Image& new_image,
				const Bitmap& bitmap,
				const CommandPool& pool
				);

	};
}


#endif //MAGMATIC_TEXTURE_HPP

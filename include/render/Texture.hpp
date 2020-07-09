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

		[[nodiscard]] DescriptorWriteUpdate getWriteInfo(size_t dst_binding, size_t dst_array_elem) const noexcept;

		[[nodiscard]] const Image &getImage() const noexcept;

		[[nodiscard]] const vk::UniqueImageView &getImageView() const noexcept;

	private:
		Image image_;
		vk::UniqueImageView image_view_;

		[[nodiscard]] static Image createImage(const LogicalDevice& l_device, const Bitmap& bitmap,
				const CommandPool& pool
				);
		[[nodiscard]] static vk::UniqueImageView createImageView(
				const LogicalDevice& l_device,
				Image& image
				);

	};
}


#endif //MAGMATIC_TEXTURE_HPP

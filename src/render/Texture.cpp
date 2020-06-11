#include "render/Texture.hpp"
#include "render/Buffer.hpp"

magmatic::render::Image magmatic::render::Texture::createImage(
		const magmatic::render::LogicalDevice& l_device,
		const magmatic::render::Bitmap& bitmap
		)
{
	const auto size = bitmap.size();
	const auto width = static_cast<uint32_t>(size.first);
	const auto height = static_cast<uint32_t>(size.second);

	return Image(
			l_device,
			vk::Extent2D(width, height),
			vk::Format::eR8G8B8A8Srgb,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::MemoryPropertyFlagBits::eDeviceLocal
	);
}

vk::UniqueImageView magmatic::render::Texture::createImageView(
		const LogicalDevice& l_device,
		Image& new_image,
		const Bitmap& bitmap,
		const CommandPool& pool
)
{
	auto buffer = Buffer::createStaging(l_device, bitmap.getPixels(), bitmap.getDataSize());

	const auto size = bitmap.size();
	const auto width = static_cast<uint32_t>(size.first);
	const auto height = static_cast<uint32_t>(size.second);

	new_image.transitionImageLayout(
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eTransferDstOptimal,
			pool
	);

	Buffer::copyBufferToImage(buffer.getBuffer(), new_image.getImage(), width, height, pool);

	new_image.transitionImageLayout(
			vk::ImageLayout::eTransferDstOptimal,
			vk::ImageLayout::eShaderReadOnlyOptimal,
			pool
	);

	return new_image.createImageView(vk::ImageAspectFlagBits::eColor, {});
}

magmatic::render::Texture::Texture(const magmatic::render::LogicalDevice& l_device,
                                   const magmatic::render::Bitmap& bitmap,
                                   const magmatic::render::CommandPool& pool
                                   )
:image(createImage(l_device, bitmap)),
image_view(createImageView(l_device, image, bitmap, pool))
{
}

magmatic::render::DescriptorWriteUpdate
magmatic::render::Texture::getWriteInfo(size_t dst_binding, size_t dst_array_elem) const {
	DescriptorWriteUpdate update;
	update.type = DescriptorWriteUpdate::eImage;
	update.dst_binding = dst_binding;
	update.dst_array_elem = dst_array_elem;
	update.data_info = std::vector{vk::DescriptorImageInfo {
			{},
			image_view.get(),
			vk::ImageLayout::eShaderReadOnlyOptimal
	}};
	return update;
}

const magmatic::render::Image& magmatic::render::Texture::getImage() const
{
	return image;
}

const vk::UniqueImageView& magmatic::render::Texture::getImageView() const
{
	return image_view;
}


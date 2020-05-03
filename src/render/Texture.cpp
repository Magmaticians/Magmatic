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

magmatic::render::Texture::Texture(const magmatic::render::LogicalDevice& l_device,
                                   const magmatic::render::Bitmap& bitmap, const magmatic::render::CommandPool& pool)
:image(std::move(createImage(l_device, bitmap)))
{
	auto buffer = Buffer::createStaging(l_device, bitmap.getPixels(), bitmap.getDataSize());

	const auto size = bitmap.size();
	const auto width = static_cast<uint32_t>(size.first);
	const auto height = static_cast<uint32_t>(size.second);

	image.transitionImageLayout(
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eTransferDstOptimal,
			pool
	);

	Buffer::copyBufferToImage(buffer.getBuffer(), image.getImage(), width, height, pool);

	image.transitionImageLayout(
			vk::ImageLayout::eTransferDstOptimal,
			vk::ImageLayout::eShaderReadOnlyOptimal,
			pool
	);

	image_view = image.createImageView(vk::ImageAspectFlagBits::eColor, {});
}

magmatic::render::DescriptorWriteUpdate
magmatic::render::Texture::getWriteInfo(size_t dst_binding, size_t dst_array_elem) const {
	DescriptorWriteUpdate update;
	update.type = DescriptorWriteUpdate::eImage;
	update.dst_binding = dst_binding;
	update.dst_array_elem = dst_array_elem;
	update.data_info = vk::DescriptorImageInfo {
			{},
			image_view.get(),
			vk::ImageLayout::eShaderReadOnlyOptimal
	};
	return update;
}





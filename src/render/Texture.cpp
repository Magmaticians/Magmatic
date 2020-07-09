#include "render/Texture.hpp"
#include "render/Buffer.hpp"

magmatic::render::Texture::Texture(const magmatic::render::LogicalDevice& l_device,
		const magmatic::render::Bitmap& bitmap,
		const magmatic::render::CommandPool& pool
)
		: image_(createImage(l_device, bitmap, pool)),
		  image_view_(createImageView(l_device, image_))
{
}

magmatic::render::Image magmatic::render::Texture::createImage(
		const magmatic::render::LogicalDevice& l_device,
		const magmatic::render::Bitmap& bitmap,
		const CommandPool& pool
		)
{
	const auto size = bitmap.size();
	const auto width = static_cast<uint32_t>(size.first);
	const auto height = static_cast<uint32_t>(size.second);

	auto usage_flags = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

	auto image =  Image(
			l_device,
			vk::Extent2D(width, height),
			vk::Format::eR8G8B8A8Srgb,
			vk::ImageTiling::eOptimal,
			usage_flags,
			vk::MemoryPropertyFlagBits::eDeviceLocal
	);

	auto buffer = Buffer::createStaging(l_device, bitmap.getPixels(), bitmap.getDataSize());

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

	return image;
}

vk::UniqueImageView magmatic::render::Texture::createImageView(
		const LogicalDevice& l_device,
		Image& image
)
{
	return image.createImageView(vk::ImageAspectFlagBits::eColor, {});
}

magmatic::render::DescriptorWriteUpdate
magmatic::render::Texture::getWriteInfo(size_t dst_binding, size_t dst_array_elem) const noexcept {
	DescriptorWriteUpdate update;
	update.type = DescriptorWriteUpdate::eImage;
	update.dst_binding = dst_binding;
	update.dst_array_elem = dst_array_elem;
	update.data_info = std::vector{vk::DescriptorImageInfo {
			{},
			image_view_.get(),
			vk::ImageLayout::eShaderReadOnlyOptimal
	}};
	return update;
}

const magmatic::render::Image& magmatic::render::Texture::getImage() const noexcept
{
	return image_;
}

const vk::UniqueImageView& magmatic::render::Texture::getImageView() const noexcept
{
	return image_view_;
}


#include <render/utils.hpp>
#include <spdlog/spdlog.h>
#include <render/CommandBuffer.hpp>
#include "render/Image.hpp"

magmatic::render::Image::Image(
		const LogicalDevice& l_device,
		vk::Extent2D extent,
		vk::Format format,
		vk::ImageTiling tiling,
		const vk::ImageUsageFlags& usage,
		const vk::MemoryPropertyFlags& memProps
)
		: image_format(format)
{
	const auto& handle = l_device.getHandle();

	vk::ImageCreateInfo image_info{
			vk::ImageCreateFlags(),
			vk::ImageType::e2D,
			format,
			{extent.width, extent.height, 1},
			1,
			1,
			vk::SampleCountFlagBits::e1,
			tiling,
			usage,
			vk::SharingMode::eExclusive
	};

	image = handle->createImageUnique(image_info);

	auto memory_requirements = handle->getImageMemoryRequirements(image.get());
	vk::MemoryAllocateInfo allocate_info{
			memory_requirements.size,
			utils::findMemoryType(
					memory_requirements.memoryTypeBits,
					memProps,
					l_device.getPhysicalDevice()
			)
	};

	memory = handle->allocateMemoryUnique(allocate_info);
	handle->bindImageMemory(image.get(), memory.get(), 0);
}

void magmatic::render::Image::transitionImageLayout(
		vk::ImageLayout old_layout,
		vk::ImageLayout new_layout,
		const CommandPool& command_pool
) {
	CommandBuffer command_buffer(command_pool);
	const auto& cmd = command_buffer.beginRecording();

	vk::ImageMemoryBarrier barrier = {};
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image.get();
	barrier.subresourceRange = {
			vk::ImageAspectFlagBits::eColor,
			0,
			1,
			0,
			1
	};

	vk::PipelineStageFlags src_stage;
	vk::PipelineStageFlags dst_stage;

	if (new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

		if (utils::hasStencilComponent(image_format)) {
			barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	} else {
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	}

	if (old_layout == vk::ImageLayout::eUndefined && new_layout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		src_stage = vk::PipelineStageFlagBits::eTopOfPipe;
		dst_stage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (old_layout == vk::ImageLayout::eTransferDstOptimal && new_layout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		src_stage = vk::PipelineStageFlagBits::eTransfer;
		dst_stage = vk::PipelineStageFlagBits::eFragmentShader;
	} else if (old_layout == vk::ImageLayout::eUndefined && new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		src_stage = vk::PipelineStageFlagBits::eTopOfPipe;
		dst_stage = vk::PipelineStageFlagBits ::eEarlyFragmentTests;
	} else {
		spdlog::error("Magmatic: Unsupported transition");
		throw std::invalid_argument("Unsupported transition");
	}

	cmd->pipelineBarrier(
			src_stage,
			dst_stage,
			{},
			nullptr, nullptr,
			barrier
	);
	command_buffer.endRecording();
	command_buffer.submitWait();
}

vk::UniqueImageView magmatic::render::Image::createImageView(const vk::ImageAspectFlags& aspect_flags,
                                                             const vk::ComponentMapping& component_mapping)
{
	const auto& l_device = image.getOwner();

	vk::ImageViewCreateInfo image_view_info = {
			vk::ImageViewCreateFlags(),
			image.get(),
			vk::ImageViewType::e2D,
			image_format,
			component_mapping,
			vk::ImageSubresourceRange {
					aspect_flags,
					0,
					1,
					0,
					1
			}
	};

	return l_device.createImageViewUnique(image_view_info);
}


vk::UniqueImageView magmatic::render::Image::createImageView(const LogicalDevice& l_device, const vk::Image& image, const vk::Format& format, const vk::ImageAspectFlags& aspect_flags, const vk::ComponentMapping& component_mapping) {
	vk::ImageViewCreateInfo image_view_info = {
			vk::ImageViewCreateFlags(),
			image,
			vk::ImageViewType::e2D,
			format,
			component_mapping,
			vk::ImageSubresourceRange {
					aspect_flags,
					0,
					1,
					0,
					1
			}
	};

	return l_device.getHandle()->createImageViewUnique(image_view_info);
}

const vk::UniqueImage &magmatic::render::Image::getImage()
{
	return image;
}

const vk::UniqueDeviceMemory &magmatic::render::Image::getMemory()
{
	return memory;
}

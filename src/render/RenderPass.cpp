#include "render/RenderPass.hpp"
#include "render/SwapChain.hpp"

magmatic::render::RenderPass::RenderPass(const LogicalDevice& l_device, const Surface& surface, const DepthResources& depthResources) {
	const auto& handle = l_device.getHandle();

	const auto& support_details = l_device.getPhysicalDevice().getSwapChainSupportDetails(surface);
	const auto surface_format = SwapChain::chooseSwapSurfaceFormat(support_details.formats);

	vk::AttachmentDescription color_attachment(
			vk::AttachmentDescriptionFlags(),
			surface_format.format,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR
	);

	vk::AttachmentReference color_attachment_reference(
			0,
			vk::ImageLayout::eColorAttachmentOptimal
	);

	vk::AttachmentDescription depthAttachment(
			vk::AttachmentDescriptionFlags(),
			depthResources.format,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentReference depthAttachmentRef(
			1,
			vk::ImageLayout::eDepthStencilAttachmentOptimal
	);

	vk::SubpassDescription subpass(
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			1,
			&color_attachment_reference,
			nullptr,
			&depthAttachmentRef
	);

	vk::SubpassDependency dependency(
			VK_SUBPASS_EXTERNAL,
			0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::AccessFlags(),
			vk::AccessFlagBits::eColorAttachmentWrite,
			vk::DependencyFlags()
	);

	std::array<vk::AttachmentDescription, 2> attachments = {color_attachment, depthAttachment};
	vk::RenderPassCreateInfo render_pass_info(
			vk::RenderPassCreateFlags(),
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			1,
			&subpass,
			1,
			&dependency
	);

	renderPass = handle->createRenderPassUnique(render_pass_info);
}
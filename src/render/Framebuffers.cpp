#include "render/Framebuffers.hpp"
#include "render/RenderPass.hpp"

magmatic::render::Framebuffers::Framebuffers(
		const LogicalDevice& l_device,
		const RenderPass& render_pass,
		const SwapChain& swapchain,
		const vk::UniqueImageView& depthImageView
) {
	const auto& handle = l_device.getHandle();

	framebuffers.reserve(swapchain.getImageViews().size());

	const auto extent = swapchain.getExtent();

	for(const auto& view: swapchain.getImageViews())
	{
		std::array<vk::ImageView, 2> attachments = {view.get(), depthImageView.get()};
		auto framebuffer = handle->createFramebufferUnique(
				vk::FramebufferCreateInfo(
						vk::FramebufferCreateFlags(),
						render_pass.getRenderPass().get(),
						static_cast<uint32_t>(attachments.size()),
						attachments.data(),
						extent.width,
						extent.height,
						1
				)
		);
		framebuffers.emplace_back(std::move(framebuffer));
	}
}

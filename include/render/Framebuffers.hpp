#ifndef MAGMATIC_FRAMEBUFFERS_HPP
#define MAGMATIC_FRAMEBUFFERS_HPP

#include <vulkan/vulkan.hpp>
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

namespace magmatic::render
{
	class RenderPass;

	class Framebuffers
	{
		std::vector<vk::UniqueFramebuffer> framebuffers;

	public:
		Framebuffers(
				const LogicalDevice& l_device,
				const RenderPass& render_pass,
				const SwapChain& swapchain,
				const vk::UniqueImageView& depthImageView
		);

		Framebuffers(const Framebuffers&) = delete;
		Framebuffers& operator=(const Framebuffers&) = delete;

		Framebuffers(Framebuffers&& rhs) noexcept;
		Framebuffers& operator=(Framebuffers&& rhs) noexcept;

		[[nodiscard]] size_t size() const { return framebuffers.size(); }
		[[nodiscard]] const vk::UniqueFramebuffer& operator[](size_t index) const {
			return framebuffers.at(index);
		}
	};
}


#endif //MAGMATIC_FRAMEBUFFERS_HPP

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
	public:
		Framebuffers(
				const LogicalDevice& l_device,
				const RenderPass& render_pass,
				const SwapChain& swapchain,
				const vk::UniqueImageView& depthImageView
		);

		Framebuffers(const Framebuffers&) = delete;
		Framebuffers& operator=(const Framebuffers&) = delete;

		Framebuffers(Framebuffers&& rhs) noexcept = default;
		Framebuffers& operator=(Framebuffers&& rhs) noexcept = default;

		~Framebuffers() = default;

		[[nodiscard]] size_t size() const { return framebuffers.size(); }
		[[nodiscard]] const vk::UniqueFramebuffer& operator[](size_t index) const {
			return framebuffers.at(index);
		}

	private:
		std::vector<vk::UniqueFramebuffer> framebuffers;
	};
}


#endif //MAGMATIC_FRAMEBUFFERS_HPP

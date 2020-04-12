#ifndef MAGMATIC_FRAMEBUFFERS_HPP
#define MAGMATIC_FRAMEBUFFERS_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic
{
	class Framebuffers
	{
		friend class LogicalDevice;
		const std::vector<vk::UniqueFramebuffer> framebuffers;

		explicit Framebuffers(std::vector<vk::UniqueFramebuffer>& framebuffers) : framebuffers(std::move(framebuffers)) {};

	public:
		Framebuffers(Framebuffers&) = delete;
		Framebuffers& operator=(Framebuffers&) = delete;
	};
}


#endif //MAGMATIC_FRAMEBUFFERS_HPP

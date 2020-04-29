#ifndef MAGMATIC_RENDERPASS_HPP
#define MAGMATIC_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>
#include "Framebuffers.hpp"

namespace magmatic::render {
    class RenderPass {
    public:
        vk::UniqueRenderPass renderPass;

        explicit RenderPass(vk::UniqueRenderPass renderPass);

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;

	    RenderPass(RenderPass&& rhs) noexcept: renderPass(std::move(rhs.renderPass)) {}
	    RenderPass& operator=(RenderPass&& rhs) noexcept;
    };
}

#endif //MAGMATIC_RENDERPASS_HPP

#ifndef MAGMATIC_RENDERPASS_HPP
#define MAGMATIC_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>
#include "Surface.hpp"
#include "DepthResources.hpp"

namespace magmatic::render {
    class RenderPass {
	    vk::UniqueRenderPass renderPass;

    public:
	    RenderPass(const LogicalDevice& l_device, const Surface& surface, const DepthResources& depthResources);

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;

	    RenderPass(RenderPass&& rhs) noexcept;
	    RenderPass& operator=(RenderPass&& rhs) noexcept;

	    [[nodiscard]] const vk::UniqueRenderPass& getRenderPass() const {
	    	return this->renderPass;
	    }
    };
}

#endif //MAGMATIC_RENDERPASS_HPP

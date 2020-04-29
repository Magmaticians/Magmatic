#ifndef MAGMATIC_RENDERPASS_HPP
#define MAGMATIC_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>
#include "Surface.hpp"
#include "DepthResources.hpp"

namespace magmatic::render {
    class RenderPass {
    public:
        vk::UniqueRenderPass renderPass;

	    magmatic::render::RenderPass::RenderPass(const LogicalDevice& l_device, const Surface& surface, const DepthResources& depthResources);

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
    };
}

#endif //MAGMATIC_RENDERPASS_HPP

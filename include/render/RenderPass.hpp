#ifndef MAGMATIC_RENDERPASS_HPP
#define MAGMATIC_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>
#include "Surface.hpp"
#include "DepthResources.hpp"

namespace magmatic::render {
    class RenderPass {
    public:
	    RenderPass(const LogicalDevice& l_device, const Surface& surface, const DepthResources& depthResources);

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;

	    RenderPass(RenderPass&& rhs) noexcept = default;
	    RenderPass& operator=(RenderPass&& rhs) noexcept = default;

	    ~RenderPass() = default;

	    [[nodiscard]] const vk::UniqueRenderPass& getRenderPass() const;

    private:
	    vk::UniqueRenderPass renderPass;
    };
}

#endif //MAGMATIC_RENDERPASS_HPP

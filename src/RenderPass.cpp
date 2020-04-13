#include "RenderPass.hpp"

magmatic::RenderPass::RenderPass(vk::UniqueRenderPass renderPass): renderPass(std::move(renderPass)) {}
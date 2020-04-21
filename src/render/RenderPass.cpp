#include "render/RenderPass.hpp"

magmatic::render::RenderPass::RenderPass(vk::UniqueRenderPass renderPass): renderPass(std::move(renderPass)) {}
#include "render/RenderPass.hpp"

magmatic::render::RenderPass::RenderPass(vk::UniqueRenderPass renderPass): renderPass(std::move(renderPass)) {}

magmatic::render::RenderPass& magmatic::render::RenderPass::operator=(RenderPass&& rhs) noexcept {
	this->renderPass = std::move(rhs.renderPass);
	return *this;
}
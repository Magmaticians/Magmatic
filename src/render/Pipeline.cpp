#include "render/Pipeline.hpp"

magmatic::render::Pipeline::Pipeline(vk::UniquePipeline pipeline): pipeline(std::move(pipeline)) {}

magmatic::render::Pipeline& magmatic::render::Pipeline::operator=(Pipeline&& rhs) noexcept {
	this->pipeline = std::move(rhs.pipeline);
	return *this;
}
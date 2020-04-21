#include "render/Pipeline.hpp"

magmatic::render::Pipeline::Pipeline(vk::UniquePipeline pipeline): pipeline(std::move(pipeline)) {}

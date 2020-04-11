#include "Pipeline.hpp"

magmatic::Pipeline::Pipeline(vk::UniquePipeline pipeline): pipeline(std::move(pipeline)) {}

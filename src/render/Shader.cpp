#include "render/Shader.hpp"

[[nodiscard]] vk::PipelineShaderStageCreateInfo magmatic::render::Shader::getPipelineShaderStageCreateInfo() const noexcept
{
	return vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			type,
			shader_module.get(),
			"main"
			);
}

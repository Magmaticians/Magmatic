#include "Shader.hpp"

[[nodiscard]] vk::PipelineShaderStageCreateInfo magmatic::Shader::getPipelineShaderStageCreateInfo() const noexcept
{
	return vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			type,
			shader_module.get(),
			"main"
			);
}

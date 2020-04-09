#include "Shader.hpp"

[[nodiscard]] vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo() const noexcept
{
	return vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			type,
			shader_module.get(),
			"main"
			);
}

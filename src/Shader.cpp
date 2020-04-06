#include "Shader.hpp"

vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits type) const noexcept
{
	return vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			type,
			shader_module.get(),
			"main"
			);
}

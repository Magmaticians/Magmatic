#ifndef MAGMATIC_SHADER_HPP
#define MAGMATIC_SHADER_HPP

#include <vulkan/vulkan.hpp>


class Shader
{
private:
	const vk::UniqueShaderModule shader_module;

public:
	explicit Shader(vk::UniqueShaderModule module) noexcept : shader_module(std::move(module)){};

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits type) const noexcept;
};


#endif //MAGMATIC_SHADER_HPP

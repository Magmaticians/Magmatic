#ifndef MAGMATIC_SHADER_HPP
#define MAGMATIC_SHADER_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic {

	class Shader {
	private:
		const vk::UniqueShaderModule shader_module;
		const vk::ShaderStageFlagBits type;

	public:
		explicit Shader(vk::UniqueShaderModule module, vk::ShaderStageFlagBits type) noexcept
				: shader_module(std::move(module)), type(type) {};

		Shader(const Shader&) = delete;
		Shader &operator=(const Shader&) = delete;

		[[nodiscard]] vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const noexcept;
	};
}

#endif //MAGMATIC_SHADER_HPP

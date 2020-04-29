#ifndef MAGMATIC_SHADER_HPP
#define MAGMATIC_SHADER_HPP

#include "render/LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>
#include <filesystem>

namespace magmatic::render {

	class Shader {
	private:
		vk::UniqueShaderModule shader_module;
		vk::ShaderStageFlagBits type;

	public:
		Shader(
				const LogicalDevice& l_device,
				const std::filesystem::path& file_path,
				vk::ShaderStageFlagBits type
				);

		Shader(const Shader&) = delete;
		Shader &operator=(const Shader&) = delete;

		[[nodiscard]] vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const noexcept;
	};
}

#endif //MAGMATIC_SHADER_HPP

#ifndef MAGMATIC_SHADER_HPP
#define MAGMATIC_SHADER_HPP

#include "render/LogicalDevice.hpp"
#include <vulkan/vulkan.hpp>
#include <filesystem>

namespace magmatic::render {

	class Shader {
	public:
		Shader(
				const LogicalDevice& l_device,
				const std::filesystem::path& file_path,
				vk::ShaderStageFlagBits type
				);

		Shader(const Shader&) = delete;
		Shader &operator=(const Shader&) = delete;

		Shader(Shader&& rhs) noexcept = default;
		Shader &operator=(Shader&& rhs) noexcept = default;

		~Shader() = default;

		[[nodiscard]] vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const noexcept;

	private:
		vk::UniqueShaderModule shader_module;
		vk::ShaderStageFlagBits type;

	};
}

#endif //MAGMATIC_SHADER_HPP

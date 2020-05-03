#include <spdlog/spdlog.h>
#include <fstream>
#include "render/Shader.hpp"

//todo: entry point
magmatic::render::Shader::Shader(
		const magmatic::render::LogicalDevice& l_device,
		const std::filesystem::path& file_path, vk::ShaderStageFlagBits type
		)
: type(type)
{
	const auto& handle = l_device.getHandle();
	if(!std::filesystem::exists(file_path))
	{
		spdlog::error("Shader file doesn't exist: {}", file_path.string());
		throw std::runtime_error("Shader file doesn't exist");
	}

	size_t file_size = std::filesystem::file_size(file_path);

	std::vector<char> spirv(file_size);

	std::ifstream spirv_file(file_path, std::ios::binary);

	if (!spirv_file.is_open()) {
		spdlog::error("Failed to open file {}", file_path.string());
		throw std::runtime_error("failed to open file!");
	}

	spirv_file.read(spirv.data(), file_size);
	spirv_file.close();

	vk::ShaderModuleCreateInfo shader_module_create_info{
			vk::ShaderModuleCreateFlags(),
			spirv.size() * sizeof(decltype(spirv)::value_type),
			reinterpret_cast<const uint32_t*>(spirv.data())
	};

	shader_module = handle->createShaderModuleUnique(shader_module_create_info);
}

[[nodiscard]] vk::PipelineShaderStageCreateInfo magmatic::render::Shader::getPipelineShaderStageCreateInfo() const noexcept
{
	return vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlags(),
			type,
			shader_module.get(),
			"main"
			);
}

magmatic::render::Shader::Shader(magmatic::render::Shader &&rhs) noexcept : shader_module(std::move(rhs.shader_module)), type(rhs.type) { }

magmatic::render::Shader &magmatic::render::Shader::operator=(magmatic::render::Shader &&rhs) noexcept {
	this->shader_module = std::move(rhs.shader_module);
	this->type = rhs.type;
	return *this;
}


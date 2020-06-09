#ifndef MAGMATIC_PIPELINE_HPP
#define MAGMATIC_PIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include "Shader.hpp"
#include "RenderPass.hpp"

namespace magmatic::render
{
	class Pipeline
	{
	public:

		Pipeline(const LogicalDevice& l_device,
				uint32_t extent_width,
				uint32_t extent_height,
				std::vector<std::reference_wrapper<Shader>> shaderStages,
				const RenderPass& renderPass,
				const vk::UniqueDescriptorSetLayout& descriptorSetLayout
		);

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

		Pipeline(Pipeline&& rhs) noexcept = default;
		Pipeline& operator=(Pipeline&& rhs) noexcept = default;

		~Pipeline() = default;

        [[nodiscard]] const vk::UniquePipelineLayout& getPipelineLayout() const;

		[[nodiscard]] const vk::UniquePipeline& getPipeline() const;

	private:
		vk::UniquePipelineLayout layout;
		vk::UniquePipeline pipeline;
	};
}


#endif //MAGMATIC_PIPELINE_HPP

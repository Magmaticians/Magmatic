#ifndef MAGMATIC_PIPELINE_HPP
#define MAGMATIC_PIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include "Shader.hpp"
#include "RenderPass.hpp"

namespace magmatic::render
{
	class Pipeline
	{
	private:

		vk::UniquePipelineLayout layout;
	public:
		vk::UniquePipeline pipeline;

		Pipeline(const LogicalDevice& l_device,
				uint32_t extent_width,
				uint32_t extent_height,
				std::vector<std::reference_wrapper<Shader>> shaderStages,
				const RenderPass& renderPass,
				const vk::UniqueDescriptorSetLayout& descriptorSetLayout
		);

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        [[nodiscard]] const vk::UniquePipelineLayout& getPipelineLayout() const {
        	return layout;
        }
	};
}


#endif //MAGMATIC_PIPELINE_HPP

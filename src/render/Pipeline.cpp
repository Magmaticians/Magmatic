#include "render/Vertex.hpp"
#include "render/PushConstantObject.h"
#include "render/Pipeline.hpp"


magmatic::render::Pipeline::Pipeline(
		const LogicalDevice& l_device,
		uint32_t extent_width, uint32_t extent_height,
		std::vector<std::reference_wrapper<Shader>> shaderStages,
		const RenderPass& renderPass,
		const vk::UniqueDescriptorSetLayout& descriptorSetLayout
) {
	const auto& handle = l_device.getHandle();

	assert((sizeof(PushConstantObject) <= l_device.getPhysicalDevice().device_properties.limits.maxPushConstantsSize));
	vk::PushConstantRange push_constant_range(vk::ShaderStageFlagBits::eVertex, 0, sizeof(PushConstantObject));

	vk::PipelineLayoutCreateInfo pipeline_layout_info(
			vk::PipelineLayoutCreateFlags(),
			1,
			&descriptorSetLayout.get(),
			1,
			&push_constant_range
	);
	layout = handle->createPipelineLayoutUnique(pipeline_layout_info);

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info(
			vk::PipelineVertexInputStateCreateFlags(),
			1,
			&bindingDescription,
			static_cast<uint32_t>(attributeDescriptions.size()),
			attributeDescriptions.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info(
			vk::PipelineInputAssemblyStateCreateFlags(),
			vk::PrimitiveTopology::eTriangleList,
			false
	);

	vk::PipelineShaderStageCreateInfo shaderStageInfos[2] = {
			shaderStages[0].get().getPipelineShaderStageCreateInfo(),
			shaderStages[1].get().getPipelineShaderStageCreateInfo()
	};

	vk::Viewport viewport(
			0.0f, 0.0f,
			static_cast<float>(extent_width), static_cast<float>(extent_height),
			0.0f, 1.0f
	);

	vk::Rect2D scissors({0,0}, {extent_width, extent_height});

	vk::PipelineViewportStateCreateInfo viewport_state_create_info(
			vk::PipelineViewportStateCreateFlags(),
			1, &viewport,
			1, &scissors
	);

	vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info(
			vk::PipelineRasterizationStateCreateFlags(),
			false,
			false,
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eBack,
			vk::FrontFace::eCounterClockwise,
			false,
			0.0f,
			0.0f,
			0.0f,
			1.0f
	);

	vk::PipelineMultisampleStateCreateInfo multisampling(
			vk::PipelineMultisampleStateCreateFlags(),
			vk::SampleCountFlagBits::e1
	);

	vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
	vk::PipelineDepthStencilStateCreateInfo depth_stencil_state(
			vk::PipelineDepthStencilStateCreateFlags(),
			true,
			true,
			vk::CompareOp::eLessOrEqual,
			false,
			false,
			stencilOpState,
			stencilOpState,
			0.0f,
			1.0f
	);

	vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
	vk::PipelineColorBlendAttachmentState pipeline_color_blend_attachment_state(
			false,
			vk::BlendFactor::eZero,
			vk::BlendFactor::eZero,
			vk::BlendOp::eAdd,
			vk::BlendFactor::eZero,
			vk::BlendFactor::eZero,
			vk::BlendOp::eAdd,
			colorComponentFlags
	);

	vk::PipelineColorBlendStateCreateInfo color_blending(
			vk::PipelineColorBlendStateCreateFlags(),
			false,
			vk::LogicOp::eNoOp,
			1, &pipeline_color_blend_attachment_state,
			{ {1.0f, 1.0f, 1.0f, 1.0f} }
	);

	vk::PipelineDynamicStateCreateInfo dynamic_states(
			vk::PipelineDynamicStateCreateFlags(),
			0,
			nullptr);

	vk::GraphicsPipelineCreateInfo pipeline_create_info(
			vk::PipelineCreateFlags(),
			2,
			shaderStageInfos,
			&vertex_input_state_create_info,
			&input_assembly_state_create_info,
			nullptr,
			&viewport_state_create_info,
			&rasterization_state_create_info,
			&multisampling,
			&depth_stencil_state,
			&color_blending,
			&dynamic_states,
			layout.get(),
			renderPass.getRenderPass().get()
	);

	pipeline = handle->createGraphicsPipelineUnique(nullptr, pipeline_create_info);
}

const vk::UniquePipeline &magmatic::render::Pipeline::getPipeline() const
{
	return pipeline;
}

const vk::UniquePipelineLayout &magmatic::render::Pipeline::getPipelineLayout() const
{
	return layout;
}

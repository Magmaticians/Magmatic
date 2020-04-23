#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "Shader.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Framebuffers.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "QueueType.hpp"
#include "Semaphores.hpp"
#include "SemaphoreType.hpp"
#include "Buffer.hpp"
#include "Vertex.hpp"
#include "DescriptorSets.hpp"
#include <vulkan/vulkan.hpp>
#include <filesystem>
#include <optional>

namespace magmatic::render
{
	class LogicalDevice
	{
	private:
		static constexpr const char* DEFAULT_EXTENSIONS[] =
				{
						VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};

		vk::UniqueDevice device;

		vk::Queue graphics_queue;
		uint32_t graphic_queue_index;
		vk::Queue present_queue;
		uint32_t present_queue_index;
		bool same_queue_family;

		const PhysicalDevice physical_dev;

	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const Surface& surface,
				const std::vector<std::string>& extensions = {}
		);

		LogicalDevice(const LogicalDevice&) = delete;

		LogicalDevice& operator=(LogicalDevice&) = delete;

		[[nodiscard]] SwapChain createSwapchain(
				const Surface& surface,
				uint32_t window_width, uint32_t window_height
		) const;

		[[nodiscard]] Shader createShader(
				const std::filesystem::path& file_path,
				vk::ShaderStageFlagBits type
		) const;

		[[nodiscard]] RenderPass createRenderPass(
				const Surface& surface
		) const;

		[[nodiscard]] vk::UniqueDescriptorSetLayout createDescriptorSetLayout() const;

		[[nodiscard]] vk::UniquePipelineLayout
		createPipelineLayout(const vk::UniqueDescriptorSetLayout& descriptorSetLayout) const;

		[[nodiscard]] Framebuffers createFramebuffers(const RenderPass& render_pass, const SwapChain& swapchain) const;

		[[nodiscard]] CommandPool createCommandPool(QueueType type) const;

		void copyBuffer(const vk::UniqueBuffer& srcBuffer, const vk::UniqueBuffer& dstBuffer, vk::DeviceSize size,
		                const CommandPool& commandPool) const;

		[[nodiscard]] std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory>
		createBuffer(vk::DeviceSize size, const vk::BufferUsageFlags& usageFlags,
		             const vk::MemoryPropertyFlags& memoryFlags) const;

		[[nodiscard]] Buffer
		createVertexBuffer(const std::vector<Vertex>& vertices, const CommandPool& commandPool) const;

		[[nodiscard]] Buffer
		createIndexBuffer(const std::vector<uint32_t>& indices, const CommandPool& commandPool) const;

		[[nodiscard]] std::vector<Buffer> createUniformBuffers(const SwapChain& swapChain) const;

		template<typename T>
		void copyBufferMemory(const vk::UniqueDeviceMemory& memory, size_t size, const T& dataToCopy) const;

		template<typename T, typename A>
		[[nodiscard]] Buffer createStagingBuffer(const std::vector<T, A>& dataToCopy) const;

		[[nodiscard]] DescriptorSets
		createDescriptorSets(const SwapChain& swapChain, const vk::UniqueDescriptorSetLayout& descriptorSetLayout,
		                     const std::vector<Buffer>& uniformBuffers) const;

		[[nodiscard]] CommandBuffer createCommandBuffer(const CommandPool& pool) const;

		[[nodiscard]] std::vector<CommandBuffer> createCommandBuffers(const CommandPool& pool, size_t count) const;

		[[nodiscard]] Semaphores createSemaphores(SemaphoreType type, size_t count) const;

		[[nodiscard]] std::vector<vk::UniqueFence> createFences(size_t count) const;

		[[nodiscard]] vk::Result acquireNextImageKHR(
				const SwapChain& swapChain, const Semaphores& imageAcquiredSemaphores,
				size_t index, uint32_t& imageIndex,
				uint64_t timeout
				) const;

		void
		submitToGraphicsQueue(const Semaphores& imageAcquiredSemaphores, const Semaphores& renderFinishedSemaphores,
		                      const CommandBuffer& commandBuffer, const vk::UniqueFence& fences, size_t index) const;

		void waitForFences(const vk::UniqueFence& fence, uint64_t timeout) const;

		void resetFences(const vk::UniqueFence& fence) const;

		void presentKHR(const Semaphores& renderFinishedSemaphores, size_t index, const SwapChain& swapChain,
		                uint32_t currentBuffer) const;

		void waitIdle() const;

		template<typename VertexType>
		[[nodiscard]] Pipeline createPipeline(
				uint32_t extent_width, uint32_t extent_height,
				std::vector<std::reference_wrapper<Shader>> shaderStages,
				const RenderPass& renderPass,
				const vk::UniquePipelineLayout& pipelineLayout
		) const;
	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
				);
	};
}

template<typename T>
void magmatic::render::LogicalDevice::copyBufferMemory(const vk::UniqueDeviceMemory& memory, size_t size, const T& dataToCopy) const {
	void* data;
	device->mapMemory(memory.get(), 0, size, vk::MemoryMapFlags(), &data);
	memcpy(data, &dataToCopy, size);
	device->unmapMemory(memory.get());
}

template<typename T, typename A>
magmatic::render::Buffer magmatic::render::LogicalDevice::createStagingBuffer(const std::vector<T, A>& dataToCopy) const{
	vk::DeviceSize bufferSize = sizeof(T) * dataToCopy.size();
	auto bufferAndMemory = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	vk::UniqueBuffer stagingBuffer = std::move(bufferAndMemory.first);
	vk::UniqueDeviceMemory stagingMemory = std::move(bufferAndMemory.second);

	void* data;
	device->mapMemory(stagingMemory.get(), 0, bufferSize, vk::MemoryMapFlags(), &data);
	memcpy(data, dataToCopy.data(), (size_t) bufferSize);
	device->unmapMemory(stagingMemory.get());

	return Buffer(std::move(stagingBuffer), std::move(stagingMemory));
}

template<typename VertexType>
magmatic::render::Pipeline magmatic::render::LogicalDevice::createPipeline(
		uint32_t extent_width, uint32_t extent_height,
		std::vector<std::reference_wrapper<Shader>> shaderStages,
		const magmatic::render::RenderPass& renderPass,
		const vk::UniquePipelineLayout& pipelineLayout
) const
{
	auto bindingDescription = VertexType::getBindingDescription();
	auto attributeDescriptions = VertexType::getAttributeDescriptions();

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
			1.0f, 1.0f
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
			stencilOpState
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
			pipelineLayout.get(),
			renderPass.renderPass.get()
	);

	vk::UniquePipeline pipeline = device->createGraphicsPipelineUnique(nullptr, pipeline_create_info);
	return Pipeline(std::move(pipeline));
}

#endif //MAGMATIC_LOGICALDEVICE_HPP
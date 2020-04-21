#include "render/LogicalDevice.hpp"
#include "render/Vertex.hpp"
#include "render/utils.hpp"
#include "render/UniformBufferObject.hpp"
#include "render/Buffer.hpp"
#include <spdlog/spdlog.h>
#include <set>
#include <fstream>
#include <functional>


magmatic::render::LogicalDevice::LogicalDevice(
		const magmatic::render::PhysicalDevice& physical_device,
		const Surface& surface,
        const std::vector<std::string>& extensions
		)
		:physical_dev(physical_device)
{
	#if !defined(NDEBUG)
	const auto available_extensions = physical_device.device.enumerateDeviceExtensionProperties();
	#endif

	std::vector<const char*> enabled_extensions;
	enabled_extensions.reserve(extensions.size()+sizeof(DEFAULT_EXTENSIONS));

	for(auto const& ext : extensions)
	{
		assert(
				std::find_if(
						available_extensions.begin(),
						available_extensions.end(),
						[ext](vk::ExtensionProperties prop)
						{
							return ext == prop.extensionName;
						}
				) != available_extensions.end()
		);
		enabled_extensions.emplace_back(ext.c_str());
	}

	for(auto const& ext : DEFAULT_EXTENSIONS)
	{
		if (
				std::find(enabled_extensions.begin(), enabled_extensions.end(), ext) == enabled_extensions.end())
		{
			assert(
					std::find_if(
							available_extensions.begin(),
							available_extensions.end(),
							[ext](const vk::ExtensionProperties prop)
							{
								return strcmp(ext, prop.extensionName);
							}
					) != available_extensions.end());
			enabled_extensions.emplace_back(ext);
		}
	}

	const auto graphic_queue_indexes = physical_device.getGraphicQueue();
	const auto present_queue_indexes = physical_device.getPresentQueue(surface);

	const auto chosen_queues = chooseGraphicPresentQueue(graphic_queue_indexes, present_queue_indexes);
	if(!chosen_queues)
	{
		spdlog::error("Failed to find queues");
		throw std::runtime_error("Failed to initialize device");
	}

	const std::set<uint32_t> unique_queues_family =
			{
					static_cast<uint32_t>(chosen_queues.value().first),
					static_cast<uint32_t>(chosen_queues.value().second)
			};

	float priority = 0.0f;

	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

	for(uint32_t family_index : unique_queues_family)
	{
		vk::DeviceQueueCreateInfo info(
				vk::DeviceQueueCreateFlags(),
				family_index,
				1,
				&priority
		);
		queue_create_infos.emplace_back(info);
	}

	const vk::DeviceCreateInfo device_create_info(
			vk::DeviceCreateFlags(),
			static_cast<uint32_t>(queue_create_infos.size()),
			queue_create_infos.data(),
			0,
			nullptr,
			static_cast<uint32_t>(enabled_extensions.size()),
			enabled_extensions.data()
	);

	device = physical_device.device.createDeviceUnique(device_create_info);

	graphic_queue_index = chosen_queues.value().first;
	present_queue_index = chosen_queues.value().second;

	graphics_queue = device->getQueue(graphic_queue_index, 0);
	present_queue = device->getQueue(present_queue_index, 0);

	same_queue_family = (graphic_queue_index == present_queue_index);
}

std::optional<std::pair<size_t, size_t>> magmatic::render::LogicalDevice::chooseGraphicPresentQueue(
		const std::vector<size_t>& graphics,
		const std::vector<size_t>& presents
		)
{
	if(graphics.empty() || presents.empty())
	{
		return std::nullopt;
	}
	for(auto i : graphics)
	{
		if(std::find(presents.begin(), presents.end(), i) != presents.end())
		{
			return std::make_pair(i, i);
		}
	}

	return std::make_pair(graphics[0], presents[0]);
}

magmatic::render::SwapChain magmatic::render::LogicalDevice::createSwapchain(
		const Surface& surface,
		uint32_t window_width, uint32_t window_height
		) const
{
	const auto& support_details = physical_dev.getSwapChainSupportDetails(surface);
	const auto& capabilities = support_details.capabilities;

	const auto surface_format = SwapChain::chooseSwapSurfaceFormat(support_details.formats);
	const auto extent = SwapChain::chooseSwapExtent(capabilities, window_width, window_height);
	const auto present_mode = SwapChain::chooseSwapPresentMode(support_details.present_modes);

	uint32_t image_count = support_details.capabilities.minImageCount + 1;

	if(capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount)
	{
		image_count = capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR swapchain_create_info(
			vk::SwapchainCreateFlagsKHR(),
			*surface.surface,
			image_count,
			surface_format.format,
			surface_format.colorSpace,
			extent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			0,
			nullptr,
			capabilities.currentTransform,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			present_mode,
			true,
			nullptr
			);

	if(!same_queue_family)
	{
		uint32_t family_indices[2] = {graphic_queue_index, present_queue_index};

		swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
		swapchain_create_info.queueFamilyIndexCount = 2;
		swapchain_create_info.pQueueFamilyIndices = family_indices;
	}

	vk::UniqueSwapchainKHR swapchain = device->createSwapchainKHRUnique(swapchain_create_info);

	return SwapChain(std::move(swapchain), device, surface_format.format, extent);
}

magmatic::render::Shader magmatic::render::LogicalDevice::createShader(const std::filesystem::path& file_path, vk::ShaderStageFlagBits type) const
{
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

	vk::UniqueShaderModule shader_module = device->createShaderModuleUnique(shader_module_create_info);

	return Shader(std::move(shader_module), type);
}

vk::UniquePipelineLayout magmatic::render::LogicalDevice::createPipelineLayout(const vk::UniqueDescriptorSetLayout& descriptorSetLayout) const {
	vk::PipelineLayoutCreateInfo pipeline_layout_info(
			vk::PipelineLayoutCreateFlags(),
			1,
			&descriptorSetLayout.get()
	);
	return device->createPipelineLayoutUnique(pipeline_layout_info);
}

magmatic::render::Pipeline magmatic::render::LogicalDevice::createPipeline(
		uint32_t extent_width, uint32_t extent_height,
		std::vector<std::reference_wrapper<Shader>> shaderStages,
		const RenderPass& renderPass,
		const vk::UniquePipelineLayout& pipelineLayout
) const
{
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

magmatic::render::RenderPass magmatic::render::LogicalDevice::createRenderPass(const Surface& surface) const {
	const auto& support_details = physical_dev.getSwapChainSupportDetails(surface);
	const auto surface_format = SwapChain::chooseSwapSurfaceFormat(support_details.formats);

	vk::AttachmentDescription color_attachment(
			vk::AttachmentDescriptionFlags(),
			surface_format.format,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR
	);

	vk::AttachmentReference color_attachment_reference(
			0,
			vk::ImageLayout::eColorAttachmentOptimal
	);

	vk::SubpassDescription subpass(
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			1,
			&color_attachment_reference,
			nullptr,
			nullptr
	);

	vk::SubpassDependency dependency(
			VK_SUBPASS_EXTERNAL,
			0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::AccessFlags(),
			vk::AccessFlagBits::eColorAttachmentWrite,
			vk::DependencyFlags()
			);

	vk::RenderPassCreateInfo render_pass_info(
			vk::RenderPassCreateFlags(),
			1,
			&color_attachment,
			1,
			&subpass,
			1,
			&dependency
	);

    vk::UniqueRenderPass renderPass = device->createRenderPassUnique(render_pass_info);

    return RenderPass(std::move(renderPass));
}

vk::UniqueDescriptorSetLayout magmatic::render::LogicalDevice::createDescriptorSetLayout() const{
	vk::DescriptorSetLayoutBinding uboLayoutBinding(
			0,
			vk::DescriptorType::eUniformBuffer,
			1,
			vk::ShaderStageFlagBits::eVertex,
			nullptr
	);
	vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(
			vk::DescriptorSetLayoutCreateFlags(),
			1,
			&uboLayoutBinding
	);
	return device->createDescriptorSetLayoutUnique(descriptor_set_layout_create_info);
}

magmatic::render::Framebuffers magmatic::render::LogicalDevice::createFramebuffers(
		const magmatic::render::RenderPass& render_pass,
		const magmatic::render::SwapChain& swapchain
		) const
{
	std::vector<vk::UniqueFramebuffer> framebuffers;
	framebuffers.reserve(swapchain.image_views_.size());

	for(const auto& view: swapchain.image_views_)
	{
		const vk::ImageView* attachment;
		attachment = &view.get();
		auto framebuffer = device->createFramebufferUnique(
				vk::FramebufferCreateInfo(
						vk::FramebufferCreateFlags(),
						render_pass.renderPass.get(),
						1,
						attachment,
						swapchain.extent.width,
						swapchain.extent.height,
						1
						)
				);
		framebuffers.emplace_back(std::move(framebuffer));
	}

	return Framebuffers(framebuffers);
}

magmatic::render::CommandPool magmatic::render::LogicalDevice::createCommandPool(QueueType type) const
{
	size_t queue_family_index;
	switch(type)
	{
		case QueueType::GraphicalQueue:
			queue_family_index = graphic_queue_index;
			break;
		case QueueType::PresentQueue:
			queue_family_index = present_queue_index;
			break;
	}
	vk::UniqueCommandPool command_pool = device->createCommandPoolUnique(
			vk::CommandPoolCreateInfo(
					vk::CommandPoolCreateFlags(),
					queue_family_index
			));
	return CommandPool(std::move(command_pool), type);
}

void magmatic::render::LogicalDevice::copyBuffer(const vk::UniqueBuffer& srcBuffer, const vk::UniqueBuffer& dstBuffer, vk::DeviceSize size, const CommandPool& commandPool) const {
	CommandBuffer commandBuffer = createCommandBuffer(commandPool);
	auto& commandBufferHandle = commandBuffer.beginRecording(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	vk::BufferCopy copyRegion(
			0,
			0,
			size
			);
	commandBufferHandle->copyBuffer(srcBuffer.get(), dstBuffer.get(), 1, &copyRegion);
	commandBuffer.endRecording();

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBufferHandle.get();
	graphics_queue.submit(1, &submitInfo, nullptr);
	graphics_queue.waitIdle();
}

std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> magmatic::render::LogicalDevice::createBuffer(vk::DeviceSize size, const vk::BufferUsageFlags& usageFlags, const vk::MemoryPropertyFlags& memoryFlags) const {
	std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> res;
	vk::BufferCreateInfo bufferCreateInfo(
			vk::BufferCreateFlags(),
			size,
			usageFlags,
			vk::SharingMode::eExclusive,
			1,
			&graphic_queue_index
	);
	res.first = device->createBufferUnique(bufferCreateInfo);
	vk::MemoryRequirements memoryRequirements = device->getBufferMemoryRequirements(res.first.get());
	vk::MemoryAllocateInfo memoryAllocateInfo(
			memoryRequirements.size,
			magmatic::render::utils::findMemoryType(memoryRequirements.memoryTypeBits,
			                                memoryFlags, physical_dev)
	);
	res.second = device->allocateMemoryUnique(memoryAllocateInfo);
	device->bindBufferMemory(res.first.get(), res.second.get(), 0);

	return res;
}

magmatic::render::Buffer magmatic::render::LogicalDevice::createVertexBuffer(const std::vector<Vertex>& vertices, const CommandPool& commandPool) const {
	Buffer stagingBuffer = createStagingBuffer(vertices);

	vk::DeviceSize bufferSize = sizeof(vertices[0])*vertices.size();
	auto bufferAndMemory = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
	vk::UniqueBuffer vertexBuffer = std::move(bufferAndMemory.first);
	vk::UniqueDeviceMemory vertexMemory = std::move(bufferAndMemory.second);

	copyBuffer(stagingBuffer.buffer, vertexBuffer, bufferSize, commandPool);

	return Buffer(std::move(vertexBuffer), std::move(vertexMemory));
}

magmatic::render::Buffer magmatic::render::LogicalDevice::createIndexBuffer(const std::vector<uint32_t>& indices, const CommandPool& commandPool) const {
	Buffer stagingBuffer = createStagingBuffer(indices);

	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	auto bufferAndMemory = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
	vk::UniqueBuffer indexBuffer = std::move(bufferAndMemory.first);
	vk::UniqueDeviceMemory indexMemory = std::move(bufferAndMemory.second);

	copyBuffer(stagingBuffer.buffer, indexBuffer, bufferSize, commandPool);

	return Buffer(std::move(indexBuffer), std::move(indexMemory));
}

[[nodiscard]] std::vector<magmatic::render::Buffer> magmatic::render::LogicalDevice::createUniformBuffers(const SwapChain& swapChain) const {
	std::vector<Buffer> uniformBuffers;
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.reserve(swapChain.images_.size());

	for(size_t i = 0; i < swapChain.images_.size(); i++) {
		auto bufferAndMemory = createBuffer(
				bufferSize,
				vk::BufferUsageFlagBits::eUniformBuffer,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		uniformBuffers.emplace_back(
				Buffer(std::move(bufferAndMemory.first), std::move(bufferAndMemory.second)));
	}
	return uniformBuffers;
}

magmatic::render::DescriptorSets magmatic::render::LogicalDevice::createDescriptorSets(
		const SwapChain& swapChain,
		const vk::UniqueDescriptorSetLayout& descriptorSetLayout,
		const std::vector<magmatic::render::Buffer>& uniformBuffers
		) const {
	auto size = static_cast<uint32_t>(swapChain.images_.size());
	vk::DescriptorPoolSize poolSize(
			vk::DescriptorType::eUniformBuffer,
			size
			);
	vk::DescriptorPoolCreateInfo poolInfo(
			vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			size,
			1,
			&poolSize
			);
	vk::UniqueDescriptorPool descriptorPool = device->createDescriptorPoolUnique(poolInfo);

	std::vector<vk::DescriptorSetLayout> layouts(size, descriptorSetLayout.get());
	vk::DescriptorSetAllocateInfo allocInfo(
			descriptorPool.get(),
			size,
			layouts.data()
			);
	DescriptorSets descriptorSets(device->allocateDescriptorSets(allocInfo), std::move(descriptorPool));

	for(size_t i = 0; i < swapChain.images_.size(); i++) {
		vk::DescriptorBufferInfo bufferInfo(
				uniformBuffers[i].buffer.get(),
				0,
				sizeof(UniformBufferObject)
				);
		vk::WriteDescriptorSet descriptorWrite(
				descriptorSets.sets[i],
				0,
				0,
				1,
				vk::DescriptorType::eUniformBuffer,
				nullptr,
				&bufferInfo,
				nullptr
				);
		device->updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
	}

	return descriptorSets;
}

std::vector<magmatic::render::CommandBuffer> magmatic::render::LogicalDevice::createCommandBuffers(const CommandPool& pool, size_t count) const
{
	vk::CommandBufferAllocateInfo command_buffer_info{
			pool.command_pool.get(),
			vk::CommandBufferLevel::ePrimary,
			static_cast<uint32_t>(count)
	};

	auto buffers = device->allocateCommandBuffersUnique(command_buffer_info);

	std::vector<CommandBuffer> resBuffers;
	resBuffers.reserve(count);

	switch(pool.type)
	{
		case QueueType::PresentQueue:
			for(auto& buffer : buffers) {
				resBuffers.emplace_back(CommandBuffer(buffer, present_queue));
			}
			return resBuffers;
		case QueueType::GraphicalQueue:
			for(auto& buffer : buffers) {
				resBuffers.emplace_back(CommandBuffer(buffer, graphics_queue));
			}
			return resBuffers;
		default:
			spdlog::error("Magmatic: Buffers for provided queue not implemented");
			throw std::runtime_error("Magmatic: Buffers for provided queue not implemented");
	}
}

magmatic::render::CommandBuffer magmatic::render::LogicalDevice::createCommandBuffer(const CommandPool& pool) const {
	return CommandBuffer(std::move(createCommandBuffers(pool, 1).front()));
}

magmatic::render::Semaphores magmatic::render::LogicalDevice::createSemaphores(SemaphoreType type, size_t count) const {
	std::vector<vk::UniqueSemaphore> semaphores;
	semaphores.reserve(count);
	for(size_t i = 0; i < count; i++) {
		semaphores.emplace_back(device->createSemaphoreUnique(vk::SemaphoreCreateInfo()));
	}
	return Semaphores(std::move(semaphores), type);
}

std::vector<vk::UniqueFence> magmatic::render::LogicalDevice::createFences(size_t count) const {
	std::vector<vk::UniqueFence> fences;
	fences.reserve(count);
	for(size_t i = 0; i < count; i++) {
		fences.emplace_back(device->createFenceUnique(vk::FenceCreateInfo()));
	}
	return fences;
}

vk::Result magmatic::render::LogicalDevice::acquireNextImageKHR(
		const SwapChain& swapChain, const Semaphores& imageAcquiredSemaphores,
		size_t index, uint32_t& imageIndex,
		uint64_t timeout = UINT64_MAX
				) const
{
	return device->acquireNextImageKHR(
			swapChain.swapchain_.get(),
			timeout,
			imageAcquiredSemaphores.semaphores[index].get(),
			nullptr,
			&imageIndex
			);
}

void magmatic::render::LogicalDevice::submitToGraphicsQueue(const Semaphores& imageAcquiredSemaphores, const Semaphores& renderFinishedSemaphores, const CommandBuffer& commandBuffer, const vk::UniqueFence& fence, size_t index) const {
	vk::PipelineStageFlags flags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	vk::SubmitInfo submitInfo(
			1,
			&imageAcquiredSemaphores.semaphores[index].get(),
			&flags,
			1,
			&commandBuffer.command_buffer.get(),
			1,
			&renderFinishedSemaphores.semaphores[index].get());
	graphics_queue.submit(submitInfo, fence.get());
}

void magmatic::render::LogicalDevice::waitForFences(const vk::UniqueFence& fence, uint64_t timeout) const {
	device->waitForFences(fence.get(), VK_TRUE, timeout);
}

void magmatic::render::LogicalDevice::resetFences(const vk::UniqueFence& fence) const {
	device->resetFences(1, &fence.get());
}

void magmatic::render::LogicalDevice::presentKHR(const Semaphores& renderFinishedSemaphores, size_t index, const SwapChain& swapChain, uint32_t currentBuffer) const {
	present_queue.presentKHR(vk::PresentInfoKHR(
			1,
			&renderFinishedSemaphores.semaphores[index].get(),
			1,
			&swapChain.swapchain_.get(),
			&currentBuffer
			));
}

void magmatic::render::LogicalDevice::waitIdle() const {
	device->waitIdle();
}
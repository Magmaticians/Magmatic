#include "LogicalDevice.hpp"
#include <spdlog/spdlog.h>
#include <set>
#include <fstream>
#include <functional>
#include <algorithm>

magmatic::LogicalDevice::LogicalDevice(
		const magmatic::PhysicalDevice& physical_device,
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

std::optional<std::pair<size_t, size_t>> magmatic::LogicalDevice::chooseGraphicPresentQueue(
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

magmatic::SwapChain magmatic::LogicalDevice::createSwapchain(
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

magmatic::Shader magmatic::LogicalDevice::createShader(const std::filesystem::path& file_path, vk::ShaderStageFlagBits type) const
{
	if(!std::filesystem::exists(file_path))
	{
		spdlog::error("Shader file not exist: {}", file_path.string());
		throw std::runtime_error("Shader file not exist");
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

magmatic::Pipeline magmatic::LogicalDevice::createPipeline(
		uint32_t extent_width, uint32_t extent_height,
		std::vector<std::reference_wrapper<Shader>> shaderStages,
		const RenderPass& renderPass
) const
{
	vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info(
			vk::PipelineVertexInputStateCreateFlags(),
			0,
			nullptr,
			0,
			nullptr
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
			vk::FrontFace::eClockwise,
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

    //TODO: Check descriptorSetLayout
    vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(
            vk::DescriptorSetLayoutCreateFlags(),
            0,
            nullptr
            );
    vk::UniqueDescriptorSetLayout descriptor_set_layout = device->createDescriptorSetLayoutUnique(descriptor_set_layout_create_info);
    vk::PipelineLayoutCreateInfo pipeline_layout_info(
            vk::PipelineLayoutCreateFlags(),
            1,
            &descriptor_set_layout.get()
            );
    vk::UniquePipelineLayout pipeline_layout = device->createPipelineLayoutUnique(pipeline_layout_info);

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
			pipeline_layout.get(),
			renderPass.renderPass.get()
	        );

	vk::UniquePipeline pipeline = device->createGraphicsPipelineUnique(nullptr, pipeline_create_info);
	return Pipeline(std::move(pipeline));
}

magmatic::RenderPass magmatic::LogicalDevice::createRenderPass(const Surface& surface) const {
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

magmatic::Framebuffers magmatic::LogicalDevice::createFramebuffers(
		const magmatic::RenderPass& render_pass,
		const magmatic::SwapChain& swapchain
		) const
{
	std::vector<vk::UniqueFramebuffer> framebuffers;
	framebuffers.reserve(swapchain.image_views_.size());

	const vk::ImageView* attachment;
	for(const auto& view: swapchain.image_views_)
	{
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

magmatic::CommandPool magmatic::LogicalDevice::createCommandPool(QueueType type) const
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

std::vector<magmatic::CommandBuffer> magmatic::LogicalDevice::createCommandBuffers(const CommandPool& pool, size_t count) const
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

magmatic::CommandBuffer magmatic::LogicalDevice::createCommandBuffer(const CommandPool& pool) const {
	return CommandBuffer(std::move(createCommandBuffers(pool, 1).front()));
}

magmatic::Semaphore magmatic::LogicalDevice::createSemaphore(SemaphoreType type) const {
	return Semaphore(device->createSemaphoreUnique(vk::SemaphoreCreateInfo()), type);
}

magmatic::Fence magmatic::LogicalDevice::createFence() const {
	return Fence(device->createFenceUnique(vk::FenceCreateInfo()));
}

vk::ResultValue<uint32_t> magmatic::LogicalDevice::getCurrentBuffer(const SwapChain& swapChain, const Semaphore& imageAcquiredSemaphore, uint64_t timeout) const {
	return device->acquireNextImageKHR(swapChain.swapchain_.get(), timeout, imageAcquiredSemaphore.semaphore.get(), nullptr);
}

void magmatic::LogicalDevice::submitToGraphicsQueue(const Semaphore& imageAcquiredSemaphore, const Semaphore& renderFinishedSemaphore, const CommandBuffer& commandBuffer, const Fence& drawFence) const {
	vk::PipelineStageFlags flags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	vk::SubmitInfo submitInfo(
			1,
			&imageAcquiredSemaphore.semaphore.get(),
			&flags,
			1,
			&commandBuffer.command_buffer.get(),
			1,
			&renderFinishedSemaphore.semaphore.get());
	graphics_queue.submit(submitInfo, drawFence.fence.get());
}

void magmatic::LogicalDevice::resetFences(const Fence& drawFence) const {
	device->resetFences(1, &drawFence.fence.get());
}

vk::Result magmatic::LogicalDevice::waitForFences(const Fence& drawFence, uint64_t timeout) const {
	return device->waitForFences(drawFence.fence.get(), VK_TRUE, timeout);
}

void magmatic::LogicalDevice::presentKHR(const Semaphore& renderFinishedSemaphore, const SwapChain& swapChain, vk::ResultValue<uint32_t> currentBuffer) const {
	present_queue.presentKHR(vk::PresentInfoKHR(
			1,
			&renderFinishedSemaphore.semaphore.get(),
			1,
			&swapChain.swapchain_.get(),
			&currentBuffer.value
			));
}

void magmatic::LogicalDevice::waitIdle() const {
	device->waitIdle();
}
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

		[[nodiscard]] Pipeline createPipeline(
				uint32_t extent_width, uint32_t extent_height,
				std::vector<std::reference_wrapper<Shader>> shaderStages,
				const RenderPass& renderPass,
				const vk::UniquePipelineLayout& pipelineLayout
		) const;

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


#endif //MAGMATIC_LOGICALDEVICE_HPP

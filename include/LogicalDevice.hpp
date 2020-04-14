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
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include <vulkan/vulkan.hpp>
#include <filesystem>
#include <optional>

namespace magmatic
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
                const std::vector<std::string>& extensions={}
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

		[[nodiscard]] Pipeline createPipeline(
				uint32_t extent_width, uint32_t extent_height,
				std::vector<std::reference_wrapper<Shader>> shaderStages,
				const RenderPass& renderPass
				) const;

		[[nodiscard]] RenderPass createRenderPass(
		        const Surface& surface
		        ) const;

		[[nodiscard]] Framebuffers createFramebuffers(const RenderPass& render_pass, const SwapChain& swapchain) const;

		[[nodiscard]] CommandPool createCommandPool(QueueType type) const;

		void copyBuffer(const vk::UniqueBuffer& srcBuffer, const vk::UniqueBuffer& dstBuffer, vk::DeviceSize size, const CommandPool& commandPool) const;
		[[nodiscard]] std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> createBuffer(vk::DeviceSize size, const vk::BufferUsageFlags& usageFlags, const vk::MemoryPropertyFlags& memoryFlags) const;
		[[nodiscard]] VertexBuffer createVertexBuffer(const std::vector<Vertex>& vertices, const CommandPool& commandPool) const;
		[[nodiscard]] VertexBuffer createStagingBuffer(const std::vector<Vertex>& vertices) const;

		[[nodiscard]] CommandBuffer createCommandBuffer(const CommandPool& pool) const;
		[[nodiscard]] std::vector<CommandBuffer> createCommandBuffers(const CommandPool& pool, size_t count) const;

		[[nodiscard]] Semaphores createSemaphores(SemaphoreType type, size_t count) const;

		[[nodiscard]] std::vector<vk::UniqueFence> createFences(size_t count) const;

		[[nodiscard]] uint32_t acquireNextImageKHR(const SwapChain& swapChain, const Semaphores& imageAcquiredSemaphores, size_t index, uint64_t timeout) const;

		void submitToGraphicsQueue(const Semaphores& imageAcquiredSemaphores, const Semaphores& renderFinishedSemaphores, const CommandBuffer& commandBuffer, const vk::UniqueFence& fences, size_t index) const;

		void waitForFences(const vk::UniqueFence& fence, uint64_t timeout) const;
		void resetFences(const vk::UniqueFence& fence) const;

		void presentKHR(const Semaphores& renderFinishedSemaphores, size_t index, const SwapChain& swapChain, uint32_t currentBuffer) const;

		void waitIdle() const;
	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
				);
	};
}


#endif //MAGMATIC_LOGICALDEVICE_HPP

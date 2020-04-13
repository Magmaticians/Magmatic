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
#include "Semaphore.hpp"
#include "SemaphoreType.hpp"
#include "Fence.hpp"
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

		[[nodiscard]] CommandBuffer createCommandBuffer(const CommandPool& pool) const;
		[[nodiscard]] std::vector<CommandBuffer> createCommandBuffers(const CommandPool& pool, size_t count) const;

		[[nodiscard]] Semaphore createSemaphore(SemaphoreType type) const;

		[[nodiscard]] Fence createFence() const;

		[[nodiscard]] vk::ResultValue<uint32_t> getCurrentBuffer(const SwapChain& swapChain, const Semaphore& imageAcquiredSemaphore, uint64_t timeout) const;

		void submitToGraphicsQueue(const Semaphore& imageAcquiredSemaphore, const Semaphore& renderFinishedSemaphore,  const CommandBuffer& commandBuffer, const Fence& drawFence) const;

		[[nodiscard]] vk::Result waitForFences(const Fence& drawFence, uint64_t timeout) const;
		void resetFences(const Fence& drawFence) const;

		void presentKHR(const Semaphore& renderFinishedSemaphore, const SwapChain& swapChain, vk::ResultValue<uint32_t> currentBuffer) const;

		void waitIdle() const;
	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
				);
	};
}


#endif //MAGMATIC_LOGICALDEVICE_HPP

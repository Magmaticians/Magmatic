#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include "Surface.hpp"

#include <vulkan/vulkan.hpp>
#include <optional>

namespace magmatic::render {
	class LogicalDevice {
	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const Surface& surface,
				const std::vector<std::string>& extensions = {}
		);

		LogicalDevice(const LogicalDevice&) = delete;
		LogicalDevice& operator=(LogicalDevice&) = delete;

		LogicalDevice(LogicalDevice&& rhs) noexcept = default;
		LogicalDevice& operator=(LogicalDevice&& rhs) noexcept = default;

		~LogicalDevice() = default;

		[[nodiscard]] const vk::UniqueDevice& getHandle() const noexcept;

		[[nodiscard]] PhysicalDevice getPhysicalDevice() const;

		[[nodiscard]] vk::Queue getGraphicsQueue() const;
		[[nodiscard]] vk::Queue getPresentQueue() const;

		[[nodiscard]] uint32_t getGraphicsQueueIndex() const;;
		[[nodiscard]] uint32_t getPresentQueueIndex() const;;

		[[nodiscard]] bool sameQueueFamily() const;

		void waitIdle() const;

	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
		);

		static constexpr const char* DEFAULT_EXTENSIONS[] =
				{
						VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};

		vk::UniqueDevice device;

		vk::Queue graphics_queue;
		uint32_t graphics_queue_index;
		vk::Queue present_queue;
		uint32_t present_queue_index;
		bool same_queue_family;

		PhysicalDevice physical_dev;

	};
}

#endif //MAGMATIC_LOGICALDEVICE_HPP

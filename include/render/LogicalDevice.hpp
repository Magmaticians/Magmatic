#ifndef MAGMATIC_LOGICALDEVICE_HPP
#define MAGMATIC_LOGICALDEVICE_HPP

#include "PhysicalDevice.hpp"
#include "Surface.hpp"

#include <vulkan/vulkan.hpp>
#include <optional>

namespace magmatic::render {
	class LogicalDevice {
	private:
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

		const PhysicalDevice physical_dev;

	public:
		explicit LogicalDevice(
				const PhysicalDevice& physical_device,
				const Surface& surface,
				const std::vector<std::string>& extensions = {}
		);

		LogicalDevice(const LogicalDevice&) = delete;

		LogicalDevice& operator=(LogicalDevice&) = delete;

		[[nodiscard]] const vk::UniqueDevice& getHandle() const noexcept
		{
			return device;
		}

		[[nodiscard]] PhysicalDevice getPhysicalDevice() const {
			return physical_dev;
		}
		[[nodiscard]] vk::Queue getGraphicsQueue() const {
			return graphics_queue;
		}
		[[nodiscard]] vk::Queue getPresentQueue() const {
			return present_queue;
		}
		[[nodiscard]] uint32_t getGraphicsQueueIndex() const {
			return graphics_queue_index;
		};
		[[nodiscard]] uint32_t getPresentQueueIndex() const {
			return present_queue_index;
		};
		[[nodiscard]] bool sameQueueFamily() const {
			return same_queue_family;
		}

		void waitIdle() const;

	private:
		static std::optional<std::pair<size_t, size_t>> chooseGraphicPresentQueue(
				const std::vector<size_t>& graphics,
				const std::vector<size_t>& presents
		);
	};
}

#endif //MAGMATIC_LOGICALDEVICE_HPP

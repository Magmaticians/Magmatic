#ifndef MAGMATIC_PHYSICALDEVICE_HPP
#define MAGMATIC_PHYSICALDEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include <fmt/format.h>


namespace magmatic::render
{
	//Forward declaration
	class Surface;

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> present_modes;
	};


	class PhysicalDevice
	{
	public:
		vk::PhysicalDevice device;
		vk::PhysicalDeviceProperties device_properties;
		vk::PhysicalDeviceFeatures device_features;
		std::vector<vk::QueueFamilyProperties> queue_family_properties;

		[[nodiscard]] uint32_t calculateScore() const noexcept;

		[[nodiscard]] std::vector<size_t> getGraphicQueue() const noexcept;
		[[nodiscard]] std::vector<size_t> getPresentQueue(const Surface& surface) const noexcept;

		[[nodiscard]] SwapChainSupportDetails getSwapChainSupportDetails(const Surface& surface) const;

		[[nodiscard]] vk::FormatProperties getFormatProperties(const vk::Format& format) const;

	private:
		friend class Instance;
		explicit PhysicalDevice(vk::PhysicalDevice physical_device);
	};
}

template <>
struct fmt::formatter<magmatic::render::PhysicalDevice>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin();
	}
	template <typename FormatContext>
	auto format(const magmatic::render::PhysicalDevice& dev, FormatContext& ctx)
	{
		const auto& name = dev.device_properties.deviceName;
		const auto& api_version = dev.device_properties.apiVersion;
		const auto device_type = vk::to_string(dev.device_properties.deviceType);

		return format_to(
				ctx.out(),
				FMT_STRING(
						"Graphic card: {}\n"
						"\tAPI version: {}\n"
						"\tDevice type: {}"
				),
				name,
				api_version,
				device_type
		);
	}
};

#endif //MAGMATIC_PHYSICALDEVICE_HPP

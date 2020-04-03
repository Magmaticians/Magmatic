#include <spdlog/spdlog.h>
#include "LogicalDevice.hpp"


magmatic::LogicalDevice::LogicalDevice(
		const magmatic::PhysicalDevice& physical_device,
		const std::vector<std::string>& extensions
		)
{
	const auto graphic_queue_indexes = physical_device.getGraphicQueue();
	if(graphic_queue_indexes.empty())
	{
		spdlog::error("Magmatic: Graphic queue not found");
		throw std::runtime_error("Failed to initialize device");
	}

	const size_t graphic_queue_index = graphic_queue_indexes[0];


	#if !defined(NDEBUG)
	const auto available_extensions = physical_device.device.enumerateDeviceExtensionProperties();
	#endif

	std::vector<const char*> enabled_extensions;
	enabled_extensions.reserve(extensions.size());

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
	const float queuePriority = 0.0f;

	const vk::DeviceQueueCreateInfo device_queue_create_info(
			vk::DeviceQueueCreateFlags(),
			graphic_queue_index,
			1,
			&queuePriority
	);
	const vk::DeviceCreateInfo device_create_info(
			vk::DeviceCreateFlags(),
			1,
			&device_queue_create_info,
			0,
			nullptr,
			static_cast<uint32_t>(enabled_extensions.size()),
			enabled_extensions.data()
	);

	device = physical_device.device.createDeviceUnique(device_create_info);

	graphics_queue = device->getQueue(graphic_queue_index, 0);
}

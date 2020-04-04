#include "LogicalDevice.hpp"
#include <spdlog/spdlog.h>
#include <set>


magmatic::LogicalDevice::LogicalDevice(
		const magmatic::PhysicalDevice& physical_device,
		const std::vector<std::string>& extensions,
		const Surface& surface
		)
{
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



	const auto graphic_queue_indexes = physical_device.getGraphicQueue();
	const auto present_queue_indexes = physical_device.getPresentQueue(surface);

	const auto chosen_queues = chooseGraphicPresentQueue(graphic_queue_indexes, present_queue_indexes);
	if(chosen_queues)
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

	graphics_queue = device->getQueue(chosen_queues.value().first, 0);
	present_queue = device->getQueue(chosen_queues.value().second, 0);

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

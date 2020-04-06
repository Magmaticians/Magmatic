#include "LogicalDevice.hpp"
#include <spdlog/spdlog.h>
#include <set>
#include <fstream>


magmatic::LogicalDevice::LogicalDevice(
		const magmatic::PhysicalDevice& physical_device,
		const std::vector<std::string>& extensions,
		const Surface& surface
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
			surface.surface.get(),
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

	return SwapChain(std::move(swapchain), device, surface_format.format);
}

Shader magmatic::LogicalDevice::createShader(const std::filesystem::path& file_path)
{
	if(!std::filesystem::exists(file_path))
	{
		spdlog::error("Shader file not exist: %s", file_path.string());
		throw std::runtime_error("Shader file not exist");
	}

	size_t file_size = std::filesystem::file_size(file_path);

	std::vector<char> spirv(file_size);

	std::ifstream spirv_file(file_path, std::ios::ate | std::ios::binary);
	spirv_file.read(spirv.data(), file_size);
	spirv_file.close();

	vk::ShaderModuleCreateInfo shader_module_create_info{
		vk::ShaderModuleCreateFlags(),
		spirv.size() * sizeof(decltype(spirv)::value_type),
		reinterpret_cast<const uint32_t*>(spirv.data())
	};

	vk::UniqueShaderModule shader_module = device->createShaderModuleUnique(shader_module_create_info);

	return Shader(std::move(shader_module));
}

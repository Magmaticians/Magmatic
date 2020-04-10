#include "Instance.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <fmt/format.h>

namespace
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback
			(
					VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
					VkDebugUtilsMessageTypeFlagsEXT msg_types,
					VkDebugUtilsMessengerCallbackDataEXT const * callback_data,
					void * user_data
			)
	{
		const auto severity = vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(msg_severity));
		const auto type = vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(msg_types));
		const auto message = fmt::format(
				"{} : {} \n"
				"\t{}\n",
				severity, type, callback_data->pMessage
				);

		if(msg_severity & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		{
			spdlog::warn(message);
		}
		else
		{
			spdlog::error(message);
		}
		return VK_TRUE;
	}
}

magmatic::Instance::Instance(
		const std::string& app_name,
		const std::vector<std::string>& extensions,
		const std::vector<std::string>& layers
		)
{
	const vk::ApplicationInfo app_info(app_name.c_str(), 1, ENGINE_NAME, 1, VK_API_VERSION_1_2);

	#if !defined(NDEBUG)
	const std::vector<vk::ExtensionProperties> available_extensions = vk::enumerateInstanceExtensionProperties();
	const std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();
	#endif

	std::vector<char const*> enabled_layers;
	enabled_layers.reserve(layers.size());

	for(auto const& layer : layers)
	{
		assert(
				std::find_if(available_layers.begin(), available_layers.end(),
				             [layer](const vk::LayerProperties& prop)
				             {
					             return layer == prop.layerName;
				             }
				) != available_layers.end()
		);
		enabled_layers.emplace_back(layer.c_str());
	}

	#if !defined(NDEBUG)
	for(auto const& layer: DEBUG_LAYERS)
	{
		if(
				std::find(enabled_layers.begin(), enabled_layers.end(), layer) == enabled_layers.end()
				&& std::find_if(available_layers.begin(), available_layers.end(),
				                [layer](const vk::LayerProperties& prop)
				                {
					                return strcmp(layer, prop.layerName) == 0;
				                }
				) != available_layers.end()
				)
		{
			enabled_layers.emplace_back(layer);
		}
	}
	#endif

	std::vector<char const*> enabled_extensions;
	enabled_extensions.reserve(extensions.size());

	for(auto const& extension : extensions)
	{
		assert(
				std::find_if(
						available_extensions.begin(), available_extensions.end(),
						[extension](const vk::ExtensionProperties& ext)
						{
							return extension == ext.extensionName;
						}
				) != available_extensions.end()
		);
		enabled_extensions.emplace_back(extension.c_str());
	}

	#if !defined(NDEBUG)
	if(
			std::find(
							enabled_extensions.begin(),
							enabled_extensions.end(),
							VK_EXT_DEBUG_UTILS_EXTENSION_NAME
					) == enabled_extensions.end()
			&& std::find_if(
							   available_extensions.begin(),
							   available_extensions.end(),
							   [](vk::ExtensionProperties const& ep)
							   {
								   return strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0;
							   }
					   ) != available_extensions.end())
	{
		enabled_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	#endif

	#if defined(NDEBUG)
	vk::StructureChain<vk::InstanceCreateInfo> instance_create_info
			({
					 {},
					 &app_info,
					 static_cast<uint32_t>(enabled_layers.size()),
					 enabled_layers.data(),
					 static_cast<uint32_t>(enabled_extensions.size()),
					 enabled_extensions.data()
			});
	#else
	const vk::DebugUtilsMessageSeverityFlagsEXT severity_flags(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
			| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
			);
	const vk::DebugUtilsMessageTypeFlagsEXT message_type_flags(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
			| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
			| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
			);

	vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instance_create_info(
			{
					{},
					&app_info,
					static_cast<uint32_t>(enabled_layers.size()),
					enabled_layers.data(),
					static_cast<uint32_t>(enabled_extensions.size()),
					enabled_extensions.data()
			},
			{
					{},
					severity_flags,
					message_type_flags,
					&debugUtilsMessengerCallback
			}
	);
	#endif
	instance = vk::createInstanceUnique(instance_create_info.get<vk::InstanceCreateInfo>());
}

std::vector<magmatic::PhysicalDevice> magmatic::Instance::enumeratePhysicalDevices() const
{
	auto to_physical_device = [](auto device)
	{
		return PhysicalDevice(device);
	};
	const auto devices = instance->enumeratePhysicalDevices();

	std::vector<PhysicalDevice> retval;
	retval.reserve(devices.size());

	std::transform(devices.begin(), devices.end(), std::back_inserter(retval), to_physical_device);

	return retval;
}

magmatic::Surface magmatic::Instance::createSurface(const magmatic::Window& window) const
{
	VkSurfaceKHR surface;
	if(glfwCreateWindowSurface(*instance, window.window.get(), nullptr, &surface) != VK_SUCCESS)
	{
		spdlog::error("Failed to create surface");
		throw std::runtime_error("Magmatic: Failed to create surface");
	}

	vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> deleter(instance.get());

	return Surface(vk::UniqueSurfaceKHR(vk::SurfaceKHR(surface), deleter));
}

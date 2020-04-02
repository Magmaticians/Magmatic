#include "Instance.hpp"
#include <string>
#include <GLFW/glfw3.h>


Instance::Instance(
		const std::string& app_name,
		const std::vector<std::string>& required_extensions,
		const std::vector<std::string>& layers
		)
{
	vk::ApplicationInfo app_info(app_name.c_str(), 1, ENGINE_NAME, 1, VK_API_VERSION_1_2);

	#if !defined(NDEBUG)
		const std::vector<vk::ExtensionProperties> available_extensions = vk::enumerateInstanceExtensionProperties();
		const std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();
	#endif

	std::vector<char const*> enabled_extensions;
	enabled_extensions.reserve(required_extensions.size());


	for(auto const& extension : required_extensions)
	{
		assert(
				std::find_if(available_extensions.begin(), available_extensions.end(),
				             [extension](const vk::ExtensionProperties& ext)
				             {
					             return extension == ext.extensionName;
				             }
				) != available_extensions.end()
		);
		enabled_extensions.push_back(extension.c_str());
	}

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
		enabled_layers.push_back(layer.c_str());
	}

	vk::InstanceCreateInfo instance_create_info
	(
			{},
			&app_info,
			static_cast<uint32_t>(enabled_layers.size()),
			enabled_layers.data(),
			static_cast<uint32_t>(enabled_extensions.size()),
			enabled_extensions.data()
			);

	instance = vk::createInstanceUnique(instance_create_info);
}

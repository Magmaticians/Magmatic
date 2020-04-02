#ifndef MAGMATIC_INSTANCE_HPP
#define MAGMATIC_INSTANCE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


class Instance
{
private:
	vk::UniqueInstance instance;

	static constexpr const char* ENGINE_NAME{"Magmatic"};
	static constexpr const char* DEBUG_LAYERS[] = {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_assistant_layer"};

public:
	explicit Instance(
			const std::string& app_name,
			const std::vector<std::string>& required_extensions = {},
			const std::vector<std::string>& layers = {}
	);
};


#endif //MAGMATIC_INSTANCE_HPP

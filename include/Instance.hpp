#ifndef MAGMATIC_INSTANCE_HPP
#define MAGMATIC_INSTANCE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


class Instance
{
private:
	vk::UniqueInstance instance;

	static constexpr const char* ENGINE_NAME{"Magmatic"};

public:
	explicit Instance(
			const std::string& app_name,
			const std::vector<std::string>& required_extensions = {},
			const std::vector<std::string>& layers = {}
	);
};


#endif //MAGMATIC_INSTANCE_HPP

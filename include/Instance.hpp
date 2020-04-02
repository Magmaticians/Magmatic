#ifndef MAGMATIC_INSTANCE_HPP
#define MAGMATIC_INSTANCE_HPP

#include <vulkan/vulkan.hpp>


class Instance
{
private:
	vk::UniqueInstance instance;

	static constexpr const char* engineName{"Magmatic"};

public:
	explicit Instance(const std::string& appName);
};


#endif //MAGMATIC_INSTANCE_HPP

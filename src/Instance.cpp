#include "Instance.hpp"
#include <string>
#include <GLFW/glfw3.h>

Instance::Instance(const std::string& appName)
{
		vk::ApplicationInfo appInfo(appName.c_str(), 1, engineName, 1, VK_API_VERSION_1_2);
		vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo);
}

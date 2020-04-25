#ifndef MAGMATIC_TEXTURE_HPP
#define MAGMATIC_TEXTURE_HPP

#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Texture
	{
	public:
		friend class LogicalDevice;
		const vk::UniqueImage image;
		const vk::UniqueDeviceMemory image_memory;

	private:
		Texture(vk::UniqueImage image, vk::UniqueDeviceMemory memory): image(std::move(image)), image_memory(std::move(memory)) {};
	};
}


#endif //MAGMATIC_TEXTURE_HPP

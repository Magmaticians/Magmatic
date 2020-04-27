#ifndef MAGMATIC_SAMPLER_HPP
#define MAGMATIC_SAMPLER_HPP


#include <vulkan/vulkan.hpp>
namespace magmatic::render
{
	class Sampler
	{
		friend class LogicalDevice;
	public:
		const vk::UniqueSampler sampler;

	private:
		explicit Sampler(vk::UniqueSampler sampler) : sampler(std::move(sampler)) {};
	};
}

#endif //MAGMATIC_SAMPLER_HPP

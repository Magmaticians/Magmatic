#ifndef MAGMATIC_PIPELINE_HPP
#define MAGMATIC_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic::render
{
	class Pipeline
	{
	private:

	public:
	    const vk::UniquePipeline pipeline;

        explicit Pipeline(vk::UniquePipeline pipeline);

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
	};
}

#endif //MAGMATIC_PIPELINE_HPP

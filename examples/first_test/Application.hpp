#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include "Window.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};
    const magmatic::Window window;
    const magmatic::Instance instance;
    const magmatic::Surface surface;
	const magmatic::PhysicalDevice physicalDevice;
	const magmatic::LogicalDevice logicalDevice;
	magmatic::Shader vertShader;
	magmatic::Shader fragShader;
	const magmatic::SwapChain swapChain;
	const magmatic::RenderPass renderPass;
	const magmatic::Pipeline pipeline;
	const magmatic::Framebuffers framebuffers;
	const magmatic::CommandPool commandPool;
	const magmatic::CommandBuffer commandBuffer;

public:
    Application();

    void run();
};


#endif //MAGMATIC_APPLICATION_H

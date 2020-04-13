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
	//const std::vector<magmatic::CommandBuffer> commandBuffers;
	magmatic::CommandBuffer commandBuffer;
	const magmatic::Fence drawFence;
	const magmatic::Semaphore imageAcquiredSemaphore;
	const magmatic::Semaphore renderFinishedSemaphore;

public:
    Application();

    void run();

private:
	static const uint64_t fenceTimeout = 1000000;

	void drawFrame();
};


#endif //MAGMATIC_APPLICATION_H

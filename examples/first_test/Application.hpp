#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include <Vertex.hpp>
#include "Window.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};
    /** Triangle */
	const std::vector<magmatic::Vertex> vertices = {
			{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	};
    /** Square? */
    /*
	const std::vector<magmatic::Vertex> vertices = {
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}
	};
     */
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
	const magmatic::VertexBuffer vertexBuffer;
	std::vector<magmatic::CommandBuffer> commandBuffers;
	const std::vector<vk::UniqueFence> fences;
	const magmatic::Semaphores imageAcquiredSemaphores;
	const magmatic::Semaphores renderFinishedSemaphores;
	std::vector<int> imagesInFlight;
public:
    Application();

    void run();

private:
	static constexpr uint64_t fenceTimeout = 1000000;
	static constexpr size_t MAX_FRAMES_IN_FLIGHT = 3;

	size_t currentFrame = 0;

	void drawFrame();
};


#endif //MAGMATIC_APPLICATION_H

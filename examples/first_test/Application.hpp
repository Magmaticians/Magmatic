#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include "render/Vertex.hpp"
#include "render/Window.hpp"
#include "render/Instance.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/SwapChain.hpp"
#include "render/Buffer.hpp"

class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};
    /** Triangle */
	const std::vector<magmatic::render::Vertex> triangleVertices = {
			{{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	};
	const std::vector<uint32_t> triangleIndices = {
			0, 1, 2
	};
	/** Square */
	const std::vector<magmatic::render::Vertex> squareVertices = {
			{{-0.8f, -0.8f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.8f, -0.8f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.8f, 0.8f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{0.8f, 0.8f, 0.0f}, {1.0f, 1.0f, 1.0f}},

			{{-0.8f, -0.8f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.8f, -0.8f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.8f, 0.8f, -0.5f}, {0.0f, 0.0f, 1.0f}},
			{{0.8f, 0.8f, -0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint32_t> squareIndices = {
			0, 1, 2, 1, 3, 2,
			4, 5, 6, 5, 7, 6,
	};
	/** Hourglass */
	const std::vector<magmatic::render::Vertex> hourglassVertices = {
			{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{-0.5f, -0.8f, 0.0f}, {0.2f, 0.2f, 1.0f}},
			{{0.5f, -0.8f, 0.0f}, {0.2f, 0.2f, 1.0f}},
			{{0.5f, 0.8f, 0.0f}, {1.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.8f, 0.0f}, {1.0f, 1.0f, 0.0f}}
	};
	const std::vector<uint32_t> hourglassIndices = {
			0, 1, 2, 0, 3, 4
	};

	const std::vector<vk::DescriptorSetLayoutBinding> bindings = {
			{
			0,
			vk::DescriptorType::eUniformBuffer,
			1,
			vk::ShaderStageFlagBits::eVertex,
			nullptr
			}
	};

	const std::vector<magmatic::render::Vertex> vertices;
	const std::vector<uint32_t> indices;

    const magmatic::render::Window window;
    const magmatic::render::Instance instance;
    const magmatic::render::Surface surface;
	const magmatic::render::PhysicalDevice physicalDevice;
	const magmatic::render::LogicalDevice logicalDevice;
	magmatic::render::Shader vertShader;
	magmatic::render::Shader fragShader;
	const magmatic::render::SwapChain swapChain;
	const magmatic::render::CommandPool commandPool;
	const magmatic::render::DepthResources depthResources;
	const magmatic::render::RenderPass renderPass;
	const vk::UniqueDescriptorSetLayout descriptorSetLayout;
	const vk::UniquePipelineLayout pipelineLayout;
	const magmatic::render::Pipeline pipeline;
	const magmatic::render::Framebuffers framebuffers;
	const magmatic::render::Buffer vertexBuffer;
	const magmatic::render::Buffer indexBuffer;
	const std::vector<magmatic::render::Buffer> uniformBuffers;
	std::vector<magmatic::render::CommandBuffer> commandBuffers;
	const magmatic::render::DescriptorSets descriptorSets;
	const std::vector<vk::UniqueFence> fences;
	const magmatic::render::Semaphores imageAcquiredSemaphores;
	const magmatic::render::Semaphores renderFinishedSemaphores;
	std::vector<int> imagesInFlight;
public:
    explicit Application(const std::string& mode);

    void run();

private:
	static constexpr uint64_t fenceTimeout = 1000000;
	static constexpr size_t MAX_FRAMES_IN_FLIGHT = 3;

	size_t currentFrame = 0;

	void updateUniformBuffer(uint32_t currentBuffer);

	[[nodiscard]] std::vector<magmatic::render::Vertex> getVertexConfig(const std::string &mode) const;

	[[nodiscard]] std::vector<uint32_t> getIndexConfig(const std::string& mode) const;
	void drawFrame();
};


#endif //MAGMATIC_APPLICATION_H

#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include <render/Texture.hpp>
#include <render/Sampler.hpp>
#include <render/DescriptorSets.hpp>
#include <render/Fences.hpp>
#include <render/VertexBuffer.hpp>
#include <render/IndexBuffer.hpp>
#include <render/Shader.hpp>
#include <render/DepthResources.hpp>
#include <render/RenderPass.hpp>
#include <render/Pipeline.hpp>
#include <render/Framebuffers.hpp>
#include <render/CommandBuffer.hpp>
#include <render/UniformBuffer.hpp>
#include "render/Vertex.hpp"
#include "render/Window.hpp"
#include "render/Instance.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/SwapChain.hpp"
#include "render/Buffer.hpp"
#include "render/UniformBufferObject.hpp"

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
			{{-0.8f, -0.8f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.8f, -0.8f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{-0.8f, 0.8f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{0.8f, 0.8f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

			{{-0.8f, -0.8f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.8f, -0.8f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{-0.8f, 0.8f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{0.8f, 0.8f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};
	const std::vector<uint32_t> squareIndices = {
			0, 1, 2, 1, 3, 2,
			4, 5, 6, 5, 7, 6,
	};
	/** Hourglass */
	const std::vector<magmatic::render::Vertex> hourglassVertices = {
			{{-0.5f, -0.5f, -0.8f}, {0.8f, 0.8f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, -0.8f}, {0.8f, 0.8f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.8f}, {0.8f, 0.8f, 1.0f}, {0.0f, 1.0f}},
			{{0.5f, -0.5f, -0.8f}, {0.8f, 0.8f, 1.0f}, {1.0f, 1.0f}},

			{{0.0f, 0.0f, 0.0f}, {0.8f, 0.8f, 1.0f}, {0.0f, 0.0f}},

			{{-0.5f, -0.5f, 0.8f}, {0.8f, 0.8f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.8f}, {0.8f, 0.8f, 1.0f}, {0.0f, 1.0f}},
			{{0.5f, 0.5f, 0.8f}, {0.8f, 0.8f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.8f}, {0.8f, 0.8f, 1.0f}, {1.0f, 0.0f}},
	};
	const std::vector<uint32_t> hourglassIndices = {
			0, 1, 2, 2, 3, 0,// lower base
			1, 0, 4, 2, 1, 4, 3, 2, 4, 0, 3, 4,// Lower faces
			5, 6, 4, 6, 7, 4, 7, 8, 4, 8, 5, 4,// Upper faces
			7, 6, 5, 5, 8, 7// Upper base
	};

	const std::vector<vk::DescriptorSetLayoutBinding> bindings = {
			{
			0,
			vk::DescriptorType::eUniformBuffer,
			1,
			vk::ShaderStageFlagBits::eVertex,
			nullptr
			},
			{
				1,
				vk::DescriptorType::eSampledImage,
				1,
				vk::ShaderStageFlagBits::eFragment,
				nullptr
			},
			{
				2,
				vk::DescriptorType::eSampler,
				1,
				vk::ShaderStageFlagBits::eFragment,
				nullptr
			}
	};

	const std::vector<vk::DescriptorType> descriptor_types = {
			vk::DescriptorType::eUniformBuffer,
			vk::DescriptorType::eSampledImage,
			vk::DescriptorType::eSampler
	};

	std::vector<magmatic::render::Vertex> vertices;
	std::vector<uint32_t> indices;

	std::unique_ptr<magmatic::render::Window> window;
	std::unique_ptr<magmatic::render::Instance> instance;
	std::unique_ptr<magmatic::render::Surface> surface;
	std::unique_ptr<magmatic::render::PhysicalDevice> physicalDevice;
	std::unique_ptr<magmatic::render::LogicalDevice> logicalDevice;
	std::unique_ptr<magmatic::render::Shader> vertShader;
	std::unique_ptr<magmatic::render::Shader> fragShader;
	std::vector<std::reference_wrapper<magmatic::render::Shader>> shaders;
	std::unique_ptr<magmatic::render::SwapChain> swapChain;
	std::unique_ptr<magmatic::render::CommandPool> commandPool;
	std::unique_ptr<magmatic::render::DepthResources> depthResources;
	std::unique_ptr<magmatic::render::RenderPass> renderPass;
	std::unique_ptr<magmatic::render::DescriptorSets> descriptorSets;
	std::unique_ptr<magmatic::render::Pipeline> pipeline;
	std::unique_ptr<magmatic::render::Framebuffers> framebuffers;
	std::unique_ptr<magmatic::render::Texture> texture;
	std::unique_ptr<magmatic::render::Sampler> sampler;
	std::unique_ptr<magmatic::render::VertexBuffer> vertexBuffer;
	std::unique_ptr<magmatic::render::IndexBuffer> indexBuffer;
	std::vector<std::unique_ptr<magmatic::render::UniformBuffer<magmatic::render::UniformBufferObject>>> uniformBuffers;
	std::vector<std::unique_ptr<magmatic::render::CommandBuffer>> commandBuffers;
	std::unique_ptr<magmatic::render::Fences> fences;
	std::unique_ptr<magmatic::render::Semaphores> imageAcquiredSemaphores;
	std::unique_ptr<magmatic::render::Semaphores> renderFinishedSemaphores;

	std::vector<int> imagesInFlight;
public:
    explicit Application(const std::string& mode);

    void run();

private:
	static constexpr uint64_t fenceTimeout = 1000000;
	size_t MAX_FRAMES_IN_FLIGHT = 3;

	size_t currentFrame = 0;
	uint32_t currentBuffer = 0;

	void updateUniformBuffer();

	[[nodiscard]] std::vector<magmatic::render::Vertex> getVertexConfig(const std::string &mode) const;

	[[nodiscard]] std::vector<uint32_t> getIndexConfig(const std::string& mode) const;
	void updateDescriptorSet();
	void recordCommandBuffer();
	void drawFrame();

	void destroySwapChain();
	void recreateSwapChain();
};


#endif //MAGMATIC_APPLICATION_H

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
#include "render/model/Model.hpp"
#include <chrono>


class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};

	std::vector<vk::DescriptorSetLayoutBinding> bindings = {
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
				32,
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
	std::unique_ptr<magmatic::render::Sampler> sampler;
	std::vector<std::unique_ptr<magmatic::render::UniformBuffer<magmatic::render::UniformBufferObject>>> uniformBuffers;
	std::vector<std::unique_ptr<magmatic::render::CommandBuffer>> commandBuffers;
	std::unique_ptr<magmatic::render::Fences> fences;
	std::unique_ptr<magmatic::render::Semaphores> imageAcquiredSemaphores;
	std::unique_ptr<magmatic::render::Semaphores> renderFinishedSemaphores;
	std::unique_ptr<magmatic::render::Model> model;
	std::vector<std::shared_ptr<magmatic::render::Texture>> textures;

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

	void updateDescriptorSets();
	void recordCommandBuffer();
	void drawFrame();

	void renderNode(
			const vk::UniqueCommandBuffer& buff,
			const std::shared_ptr<magmatic::render::ModelData::NodeData>& node,
			glm::mat4 parent_matrix
			);

	void destroySwapChain();
	void recreateSwapChain();

	glm::vec3 position = glm::vec3(4.0f, 0.0f, 0.0f);
	glm::vec3 offset = glm::vec3(4.0f, 0.0f, 0.0f);

	static constexpr float speed = 1.5f;

	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame;
	float deltaTime = 0.0f;
	// TODO: Extract to external class
	bool right = false;
	bool left = false;
	bool forward = false;
	bool backward = false;
	bool up = false;
	bool down = false;
	void moveLeft();
	void moveRight();
	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();

	void move();

	void moveBindings(int key, int action);

	void setMoveBindings();
};


#endif //MAGMATIC_APPLICATION_H

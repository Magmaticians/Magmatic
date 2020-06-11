#define GLM_FORCE_RADIANS
#include "Application.hpp"
#include "render/UniformBufferObject.hpp"
#include "render/model/ModelData.hpp"
#include "render/model/model_data_loader/formats/ModelDataLoadergltf.hpp"
#include "ActionHandlers.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include <render/CommandBuffer.hpp>
#include <render/PushConstantObject.h>
#include <utils/factory/FileLoaderFactory.hpp>
#include <sound/formats/SoundLoaderVorbis.hpp>


Application::Application(const std::string& mode):
	quackSource({0, 0, 0}),
	musicSource({0, 0, 0}) {
	spdlog::info("Loaders registered");

	auto quackBuffer = magmatic::sound::SoundLoader::load("VORBIS", "examples/resources/Duck-quack.ogg");
	quackSource.setSound(quackBuffer);
	auto musicBuffer = magmatic::sound::SoundLoader::load("VORBIS", "examples/resources/bit-sound-mono.ogg");
	musicSource.setSound(musicBuffer);
	musicSource.setGain(0.2f);
	musicSource.setRepeats(true);
	spdlog::info("Initialized sound");

	window = std::make_unique<magmatic::render::Window>(DEFAULT_NAME);
	instance = std::make_unique<magmatic::render::Instance>(DEFAULT_NAME, window->getRequiredExtensions());
	surface = std::make_unique<magmatic::render::Surface>(instance->createSurface(*window));
	spdlog::info("Created window and surface");

	physicalDevice = std::make_unique<magmatic::render::PhysicalDevice>(instance->getBestDevice());
	logicalDevice = std::make_unique<magmatic::render::LogicalDevice>(*physicalDevice, *surface);
	spdlog::info("Created devices");

	vertShader = std::make_unique<magmatic::render::Shader>(*logicalDevice, "./examples/first_test/vert.spv", vk::ShaderStageFlagBits::eVertex);
	fragShader = std::make_unique<magmatic::render::Shader>(*logicalDevice, "./examples/first_test/frag.spv", vk::ShaderStageFlagBits::eFragment);
	shaders = {*vertShader, *fragShader};
	spdlog::info("Created shaders");

	swapChain = std::make_unique<magmatic::render::SwapChain>(*logicalDevice, *surface, *window);
	MAX_FRAMES_IN_FLIGHT = (swapChain->getImages().size());
	commandPool = std::make_unique<magmatic::render::CommandPool>(*logicalDevice, magmatic::render::QueueType::GraphicalQueue);

	{
		auto model_data = magmatic::render::ModelDataLoader::load("GLTF", "examples/resources/Duck.gltf");
		model = std::make_unique<magmatic::render::Model>(*logicalDevice, *commandPool, model_data);
	}

	textures.reserve(model->textures.size());
	std::copy(model->textures.begin(), model->textures.end(), std::back_inserter(textures));
	spdlog::info("Created textures");

	depthResources = std::make_unique<magmatic::render::DepthResources>(*logicalDevice, swapChain->getExtent(), *commandPool);
	renderPass = std::make_unique<magmatic::render::RenderPass>(*logicalDevice, *surface, *depthResources);
	descriptorSets = std::make_unique<magmatic::render::DescriptorSets>(*logicalDevice, bindings, MAX_FRAMES_IN_FLIGHT);
	pipeline = std::make_unique<magmatic::render::Pipeline>(*logicalDevice, swapChain->getExtent().width, swapChain->getExtent().height, shaders, *renderPass, descriptorSets->getDescriptorSetLayout());
	framebuffers = std::make_unique<magmatic::render::Framebuffers>(*logicalDevice, *renderPass, *swapChain, depthResources->getImageView());
	spdlog::info("Created ???");

	sampler = std::make_unique<magmatic::render::Sampler>(*logicalDevice);

	commandBuffers = magmatic::render::CommandBuffer::createCommandBuffersUnique(MAX_FRAMES_IN_FLIGHT, *commandPool);
	uniformBuffers = magmatic::render::UniformBuffer<magmatic::render::UniformBufferObject>::createUniformBuffersUnique(MAX_FRAMES_IN_FLIGHT, *logicalDevice, *commandPool);
	spdlog::info("Created buffers");



	fences = std::make_unique<magmatic::render::Fences>(*logicalDevice, MAX_FRAMES_IN_FLIGHT);
	imageAcquiredSemaphores = std::make_unique<magmatic::render::Semaphores>(*logicalDevice, magmatic::render::SemaphoreType::ImageAvailableSemaphore, MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores = std::make_unique<magmatic::render::Semaphores>(*logicalDevice, magmatic::render::SemaphoreType::RenderFinishedSemaphore, MAX_FRAMES_IN_FLIGHT);
	spdlog::info("Created sync objects and thus everything");

	setMoveBindings();
}

void Application::destroySwapChain() {
	depthResources.reset();
	framebuffers.reset();
	pipeline.reset();
	renderPass.reset();
	swapChain.reset();
	descriptorSets.reset();
}

void Application::recreateSwapChain() {
	std::pair<int, int> size = window->getSize();
	while(size.first == 0 || size.second == 0) {
		size = window->getSize();
		glfwWaitEvents();
	}

	logicalDevice->waitIdle();

	destroySwapChain();

	swapChain = std::make_unique<magmatic::render::SwapChain>(magmatic::render::SwapChain(*logicalDevice, *surface, *window));
	MAX_FRAMES_IN_FLIGHT = (swapChain->getImages().size());
	depthResources = std::make_unique<magmatic::render::DepthResources>(*logicalDevice,swapChain->getExtent(), *commandPool);
	renderPass = std::make_unique<magmatic::render::RenderPass>(*logicalDevice, *surface, *depthResources);
	descriptorSets = std::make_unique<magmatic::render::DescriptorSets>(*logicalDevice, bindings, MAX_FRAMES_IN_FLIGHT);
	pipeline = std::make_unique<magmatic::render::Pipeline>(*logicalDevice, swapChain->getExtent().width, swapChain->getExtent().height, shaders, *renderPass, descriptorSets->getDescriptorSetLayout());
	framebuffers = std::make_unique<magmatic::render::Framebuffers>(*logicalDevice, *renderPass, *swapChain, depthResources->getImageView());

	imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, -1);
	currentBuffer = 0;
	currentFrame = 0;

	updateDescriptorSets();
}

void Application::run() {
	imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, -1);
	currentBuffer = 0;
	currentFrame = 0;
	lastFrame = std::chrono::high_resolution_clock::now();

	updateDescriptorSets();

	musicSource.play();
	while(!window->shouldClose()) {
		glfwPollEvents();
		drawFrame();
		move();
		auto time = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(time - lastFrame).count();
		lastFrame = time;
	}

	logicalDevice->waitIdle();
}

void Application::updateUniformBuffer() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	magmatic::render::UniformBufferObject ubo = {};
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	ubo.model = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), time*glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.01f, 0.01f, 0.01f));
	ubo.view = glm::lookAt(position, position - offset, glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->getExtent().width/(float) swapChain->getExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	uniformBuffers[currentBuffer]->copyMemory(sizeof(ubo), ubo);
}

void Application::updateDescriptorSets() {
	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::vector<magmatic::render::DescriptorWriteUpdate> updates;
		magmatic::render::DescriptorWriteUpdate write_update;
		write_update.type = magmatic::render::DescriptorWriteUpdate::eUniform;
		write_update.dst_binding = 0;
		write_update.dst_array_elem = 0;
		vk::DescriptorBufferInfo info(
				uniformBuffers[currentFrame]->getBuffer().get(),
				0,
				sizeof(magmatic::render::UniformBufferObject)
		);
		write_update.data_info = std::vector<vk::DescriptorBufferInfo>{info};
		updates.emplace_back(write_update);
		assert((textures.size() < 32));
		vk::DescriptorImageInfo descriptorImageInfos;
			descriptorImageInfos.sampler = nullptr;
			descriptorImageInfos.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			descriptorImageInfos.imageView = *(textures[0]->getImageView());
		write_update = {};
		write_update.dst_binding = 1;
		write_update.dst_array_elem = 0;
		write_update.type = magmatic::render::DescriptorWriteUpdate::eImage;
		write_update.data_info = std::vector{descriptorImageInfos};
		updates.emplace_back(write_update);
		updates.emplace_back(sampler->getWriteInfo(2, 0));
		descriptorSets->updateDescriptorSet(i, updates);
	}
}

void Application::recordCommandBuffer() {
	vk::Buffer vertexBuffers[] = { model->vertex_buffer->getBuffer().get() };
	vk::Buffer indexBuffers{ model->index_buffer->getBuffer().get() };
	vk::DeviceSize offsets[] = { 0 };

	const auto& commandBufferHandle = commandBuffers[currentFrame]->beginRecording();
	std::array<vk::ClearValue, 2> clearValues;
	clearValues[0].color =  vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.0f}));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
	vk::RenderPassBeginInfo beginInfo(renderPass->getRenderPass().get(),
	                                  (*framebuffers)[currentBuffer].get(),
	                                  vk::Rect2D(vk::Offset2D(0, 0), swapChain->getExtent()),
	                                  static_cast<uint32_t>(clearValues.size()),
	                                  clearValues.data());
	commandBufferHandle->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
	commandBufferHandle->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->getPipeline().get());
	commandBufferHandle->bindVertexBuffers(0, 1, vertexBuffers, offsets);
	commandBufferHandle->bindIndexBuffer(indexBuffers, 0, vk::IndexType::eUint32);
	commandBufferHandle->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->getPipelineLayout().get(), 0, 1, &(*descriptorSets)[currentFrame].get(), 0, nullptr);

	for(const auto& node : model->nodes)
	{
		renderNode(commandBufferHandle, node, glm::mat4(1.0f));
	}
	commandBufferHandle->endRenderPass();
	commandBuffers[currentFrame]->endRecording();

}

void Application::drawFrame() {
	vk::Result state;
	try {
		state = swapChain->acquireNextImageKHR(*imageAcquiredSemaphores, currentFrame, currentBuffer, 0);
	} catch (vk::OutOfDateKHRError&) {
		recreateSwapChain();
		return;
	}
	if(state == vk::Result::eNotReady)
	{
		std::this_thread::yield();
		return;
	} else if(state != vk::Result::eSuccess && state != vk::Result::eSuboptimalKHR) {
		spdlog::error("Failed to acquire swap chain image!");
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	if(imagesInFlight[currentBuffer] != -1) {
		fences->waitForFence(imagesInFlight[currentBuffer], fenceTimeout);
		fences->resetFence(currentFrame);
	}
	imagesInFlight[currentBuffer] = currentFrame;
	recordCommandBuffer();

	updateUniformBuffer();
	commandBuffers[currentBuffer]->submit(*imageAcquiredSemaphores, *renderFinishedSemaphores, (*fences)[currentFrame], currentFrame);
	try {
		state = swapChain->presentKHR(*logicalDevice, *renderFinishedSemaphores, currentFrame, currentBuffer);
	} catch(vk::OutOfDateKHRError&) {
		recreateSwapChain();
		return;
	}
	if(state == vk::Result::eSuboptimalKHR) {
		recreateSwapChain();
		return;
	} else if(state != vk::Result::eSuccess) {
		spdlog::error("Failed to acquire swap chain image!");
		throw std::runtime_error("Failed to acquire swap chain image!");
	}
	currentFrame = (currentFrame+1)%MAX_FRAMES_IN_FLIGHT;
}

void Application::renderNode(
		const vk::UniqueCommandBuffer &buff,
		const std::shared_ptr<magmatic::render::ModelData::NodeData> &node,
		glm::mat4 parent_matrix
)
{
	glm::mat4 current_matrix = parent_matrix * node->matrix;
	magmatic::render::PushConstantObject obj{};
	obj.matrix = current_matrix;
	for(const auto& primitive : node->mesh_data)
	{
		obj.sampler_index = 0;
		obj.texture_index = model->texture_data[model->material_data[primitive.material_index].texture_index].bitmap_index;
		buff->pushConstants(
				pipeline->getPipelineLayout().get(),
				vk::ShaderStageFlagBits::eVertex,
				0, sizeof(magmatic::render::PushConstantObject),
				&obj
				);
		buff->drawIndexed(primitive.index_count, 1u, primitive.first_index, 0, 0);
	}
	for(const auto& child : node->children)
	{
		renderNode(buff, child, current_matrix);
	}
}


void Application::moveLeft() {
	position += deltaTime*glm::vec3(0.0f, -speed, 0.0f);
}
void Application::moveRight() {
	position += deltaTime*glm::vec3(0.0f, speed, 0.0f);
}
void Application::moveForward() {
	position += deltaTime*glm::vec3(-speed, 0.0f, 0.0f);
}
void Application::moveBackward() {
	position += deltaTime*glm::vec3(speed, 0.0f, 0.0f);
}
void Application::moveUp() {
	position += deltaTime*glm::vec3(0.0f, 0.0f, speed);
}
void Application::moveDown() {
	position += deltaTime*glm::vec3(0.0f, 0.0f, -speed);
}

void Application::move() {
	if(right)
		moveRight();
	if(left)
		moveLeft();
	if(forward)
		moveForward();
	if(backward)
		moveBackward();
	if(up)
		moveUp();
	if(down)
		moveDown();
}
// TODO: Create custom cursor (glfw)
// TODO: Scroll = change distance
void Application::moveBindings(int key, int action) {
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_A:
				left = true;
				break;
			case GLFW_KEY_D:
				right = true;
				break;
			case GLFW_KEY_W:
				forward = true;
				break;
			case GLFW_KEY_S:
				backward = true;
				break;
			case GLFW_KEY_SPACE:
				up = true;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				down = true;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				speed = sprintSpeed;
				break;
			case GLFW_KEY_P:
				quackSource.play();
				break;
		}
	}
	if(action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_A:
				left = false;
				break;
			case GLFW_KEY_D:
				right = false;
				break;
			case GLFW_KEY_W:
				forward = false;
				break;
			case GLFW_KEY_S:
				backward = false;
				break;
			case GLFW_KEY_SPACE:
				up = false;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				down = false;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				speed = normalSpeed;
				break;
		}
	}
}

void Application::setMoveBindings() {
	ActionHandlers::registerKeyboardObserver([&](int key, int action) {
		moveBindings(key, action);
	});

	glfwSetKeyCallback(window->getWindow().get(), ActionHandlers::keyHandler);
	//gflwSetScrollCallback(window->getWindow().get(), )
}

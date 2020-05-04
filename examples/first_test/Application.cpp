#define GLM_FORCE_RADIANS
#include <algorithm>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include "render/UniformBufferObject.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/Bitmap.hpp>
#include "Application.hpp"
#include "ActionHandlers.hpp"

Application::Application(const std::string& mode) {
	vertices = getVertexConfig(mode);
	indices = getIndexConfig(mode);
	spdlog::info("Chose vertices and indices");

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
	commandPool = std::make_unique<magmatic::render::CommandPool>(*logicalDevice, magmatic::render::QueueType::GraphicalQueue);
	depthResources = std::make_unique<magmatic::render::DepthResources>(*logicalDevice, swapChain->extent, *commandPool);
	renderPass = std::make_unique<magmatic::render::RenderPass>(*logicalDevice, *surface, *depthResources);
	descriptorSets = std::make_unique<magmatic::render::DescriptorSets>(*logicalDevice, bindings, MAX_FRAMES_IN_FLIGHT, descriptor_types);
	pipeline = std::make_unique<magmatic::render::Pipeline>(*logicalDevice, swapChain->extent.width, swapChain->extent.height, shaders, *renderPass, descriptorSets->getDescriptorSetLayout());
	framebuffers = std::make_unique<magmatic::render::Framebuffers>(*logicalDevice, *renderPass, *swapChain, depthResources->imageView);
	spdlog::info("Created ???");

	texture = std::make_unique<magmatic::render::Texture>(*logicalDevice, magmatic::render::Bitmap("examples/resources/statue.jpg"), *commandPool);
	sampler = std::make_unique<magmatic::render::Sampler>(*logicalDevice);
	spdlog::info("Created textures");

	vertexBuffer = std::make_unique<magmatic::render::VertexBuffer>(*logicalDevice, vertices, *commandPool);
	indexBuffer = std::make_unique<magmatic::render::IndexBuffer>(*logicalDevice, indices, *commandPool);
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
	depthResources = std::make_unique<magmatic::render::DepthResources>(*logicalDevice,swapChain->extent, *commandPool);
	renderPass = std::make_unique<magmatic::render::RenderPass>(*logicalDevice, *surface, *depthResources);
	descriptorSets = std::make_unique<magmatic::render::DescriptorSets>(*logicalDevice, bindings, MAX_FRAMES_IN_FLIGHT, descriptor_types);
	pipeline = std::make_unique<magmatic::render::Pipeline>(*logicalDevice, swapChain->extent.width, swapChain->extent.height, shaders, *renderPass, descriptorSets->getDescriptorSetLayout());
	framebuffers = std::make_unique<magmatic::render::Framebuffers>(*logicalDevice, *renderPass, *swapChain, depthResources->imageView);

	imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, -1);
	currentBuffer = 0;
	currentFrame = 0;
	MAX_FRAMES_IN_FLIGHT = std::min(static_cast<size_t>(5), swapChain->images_.size());

	updateDescriptorSets();
}

void Application::run() {
	imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, -1);
	currentBuffer = 0;
	currentFrame = 0;
	MAX_FRAMES_IN_FLIGHT = std::min(static_cast<size_t>(5), swapChain->images_.size());
	lastFrame = std::chrono::high_resolution_clock::now();


	updateDescriptorSets();

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
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(position, position - offset, glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->extent.width/(float) swapChain->extent.height, 0.1f, 10.0f);
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
		write_update.data_info = info;
		updates.emplace_back(write_update);

		updates.emplace_back(texture->getWriteInfo(1, 0));
		updates.emplace_back(sampler->getWriteInfo(2, 0));
		descriptorSets->updateDescriptorSet(i, updates);
	}
}

void Application::recordCommandBuffer() {
	vk::Buffer vertexBuffers[] = { vertexBuffer->getBuffer().get() };
	vk::DeviceSize offsets[] = { 0 };

	const auto& commandBufferHandle = commandBuffers[currentFrame]->beginRecording();
	std::array<vk::ClearValue, 2> clearValues;
	clearValues[0].color =  vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.0f}));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
	vk::RenderPassBeginInfo beginInfo(renderPass->getRenderPass().get(),
	                                  (*framebuffers)[currentBuffer].get(),
	                                  vk::Rect2D(vk::Offset2D(0, 0), swapChain->extent),
	                                  static_cast<uint32_t>(clearValues.size()),
	                                  clearValues.data());
	commandBufferHandle->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
	commandBufferHandle->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->getPipeline().get());
	commandBufferHandle->bindVertexBuffers(0, 1, vertexBuffers, offsets);
	commandBufferHandle->bindIndexBuffer(indexBuffer->getBuffer().get(), 0, vk::IndexType::eUint32);
	commandBufferHandle->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->getPipelineLayout().get(), 0, 1, &(*descriptorSets)[currentFrame].get(), 0, nullptr);
	commandBufferHandle->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
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

std::vector<magmatic::render::Vertex> Application::getVertexConfig(const std::string& mode) const {
	if(mode == "square") {
		return squareVertices;
	} else if(mode == "triangle") {
		return triangleVertices;
	} else if(mode == "hourglass") {
		return hourglassVertices;
	} else if(mode == "hourglass on square") {
		std::vector<magmatic::render::Vertex> res;
		res.reserve(hourglassVertices.size() + squareVertices.size());
		res.insert(res.end(), squareVertices.begin(), squareVertices.end());
		res.insert(res.end(), hourglassVertices.begin(), hourglassVertices.end());
		return res;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
	}
}

std::vector<uint32_t> Application::getIndexConfig(const std::string& mode) const {
	if(mode == "square") {
		return squareIndices;
	} else if(mode == "triangle") {
		return triangleIndices;
	} else if(mode == "hourglass") {
		return hourglassIndices;
	} else if(mode == "hourglass on square") {
		std::vector<uint32_t> res;
		res.reserve(hourglassIndices.size() + squareIndices.size());
		res.insert(res.end(), squareIndices.begin(),squareIndices.end());
		for(uint32_t index : hourglassIndices) {
			res.emplace_back(index + static_cast<uint32_t>(squareVertices.size()));
		}
		return res;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
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
			case GLFW_KEY_LEFT_SHIFT:
				down = true;
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
			case GLFW_KEY_LEFT_SHIFT:
				down = false;
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

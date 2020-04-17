#include <cctype>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include "Application.hpp"

Application::Application(const std::string& mode):
vertices(std::move(getVertexConfig(mode))),
indices(std::move(getIndexConfig(mode))),
window(magmatic::Window(DEFAULT_NAME)),
instance(magmatic::Instance(DEFAULT_NAME, window.getRequiredExtensions())),
surface(instance.createSurface(window)),
physicalDevice(magmatic::PhysicalDevice(instance.getBestDevice())),
logicalDevice(magmatic::LogicalDevice(physicalDevice, surface)),
vertShader(logicalDevice.createShader("./examples/first_test/vert.spv", vk::ShaderStageFlagBits::eVertex)),
fragShader(logicalDevice.createShader("./examples/first_test/frag.spv", vk::ShaderStageFlagBits::eFragment)),
swapChain(logicalDevice.createSwapchain(surface, window.getSize().first, window.getSize().second)),
renderPass(logicalDevice.createRenderPass(surface)),
pipeline(logicalDevice.createPipeline(swapChain.extent.width, swapChain.extent.height, {vertShader, fragShader}, renderPass)),
framebuffers(logicalDevice.createFramebuffers(renderPass, swapChain)),
commandPool(logicalDevice.createCommandPool(magmatic::QueueType::GraphicalQueue)),
vertexBuffer(logicalDevice.createVertexBuffer(vertices, commandPool)),
indexBuffer(logicalDevice.createIndexBuffer(indices, commandPool)),
commandBuffers(logicalDevice.createCommandBuffers(commandPool, framebuffers.getSize())),
fences(logicalDevice.createFences(MAX_FRAMES_IN_FLIGHT)),
imageAcquiredSemaphores(logicalDevice.createSemaphores(magmatic::SemaphoreType::ImageAvailableSemaphore, MAX_FRAMES_IN_FLIGHT)),
renderFinishedSemaphores(logicalDevice.createSemaphores(magmatic::SemaphoreType::RenderFinishedSemaphore, MAX_FRAMES_IN_FLIGHT)){
	spdlog::info("Application constructor called and finished work");
}

void Application::run() {
	currentFrame = 0;
	imagesInFlight.resize(swapChain.images_.size(), -1);

	vk::Buffer vertexBuffers[] = { vertexBuffer.buffer.get() };
	vk::DeviceSize offsets[] = { 0 };

	for(size_t i = 0; i < commandBuffers.size(); i++) {
		auto& commandBufferHandle = std::move(commandBuffers[i].beginRecording(vk::CommandBufferUsageFlagBits::eSimultaneousUse));
		vk::ClearValue clearValues[2];
		clearValues[0].color =  vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.0f}));
		clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
		vk::RenderPassBeginInfo beginInfo(renderPass.renderPass.get(),
		                                  framebuffers.framebuffers[i].get(),
		                                  vk::Rect2D(vk::Offset2D(0, 0), swapChain.extent),
		                                  2,
		                                  clearValues);
		commandBufferHandle->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
		commandBufferHandle->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline.get());
		commandBufferHandle->bindVertexBuffers(0, 1, vertexBuffers, offsets);
		commandBufferHandle->bindIndexBuffer(indexBuffer.buffer.get(), 0, vk::IndexType::eUint32);
		commandBufferHandle->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		commandBufferHandle->endRenderPass();
		commandBuffers[i].endRecording();
	}

	while(!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}

	logicalDevice.waitIdle();
}

void Application::drawFrame() {
	logicalDevice.waitForFences(fences[currentFrame], fenceTimeout);
	uint32_t currentBuffer = logicalDevice.acquireNextImageKHR(swapChain, imageAcquiredSemaphores, currentFrame, fenceTimeout);
	if(imagesInFlight[currentBuffer] != -1) {
		logicalDevice.waitForFences(fences[imagesInFlight[currentBuffer]], fenceTimeout);
	}
	imagesInFlight[currentBuffer] = currentFrame;
	logicalDevice.resetFences(fences[currentFrame]);
	logicalDevice.submitToGraphicsQueue(imageAcquiredSemaphores, renderFinishedSemaphores, commandBuffers[currentBuffer], fences[currentFrame], currentFrame);
	logicalDevice.presentKHR(renderFinishedSemaphores, currentFrame, swapChain, currentBuffer);
	currentFrame = (currentFrame+1)%MAX_FRAMES_IN_FLIGHT;
}

std::vector<magmatic::Vertex> Application::getVertexConfig(const std::string& mode) const {
	if(mode == "square") {
		return squareVertices;
	} else if(mode == "triangle") {
		return triangleVertices;
	} else if(mode == "hourglass") {
		return hourglassVertices;
	} else if(mode == "hourglass on square") {
		std::vector<magmatic::Vertex> res;
		res.reserve(hourglassVertices.size() + squareVertices.size());
		res.insert(res.end(), squareVertices.begin(), squareVertices.end());
		res.insert(res.end(), hourglassVertices.begin(), hourglassVertices.end());
		return res;
	} else if (mode == "indexed square"){
		return indexedSquareVertices;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
	}
}

std::vector<uint32_t> Application::getIndexConfig(const std::string& mode) const {
	if (mode == "indexed square"){
		return squareIndices;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
	}
}
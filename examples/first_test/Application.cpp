#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include "Application.hpp"

Application::Application():window(magmatic::Window(DEFAULT_NAME)),
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
commandBuffer(logicalDevice.createCommandBuffer(commandPool)),
fences(logicalDevice.createFences(MAX_FRAMES_IN_FLIGHT)),
//imagesInFlight(logicalDevice.createFences(swapChain.images_.size())),
imageAcquiredSemaphores(logicalDevice.createSemaphores(magmatic::SemaphoreType::ImageAvailableSemaphore, MAX_FRAMES_IN_FLIGHT)),
renderFinishedSemaphores(logicalDevice.createSemaphores(magmatic::SemaphoreType::RenderFinishedSemaphore, MAX_FRAMES_IN_FLIGHT)){
	spdlog::info("Application constructor called and finished work");
}

void Application::run() {
	currentFrame = 0;

	commandBuffer.beginRecording(/*vk::CommandBufferUsageFlags()*/);
	commandBuffer.beginRenderPass(renderPass, framebuffers, swapChain.extent);
	commandBuffer.bindPipeline(pipeline);
	//commandBuffer.setViewport(swapChain.extent);
	//commandBuffer.setScissor(swapChain.extent);
	commandBuffer.draw(3, 1, 0, 0);
	commandBuffer.endRenderPass();
	commandBuffer.endRecording();

	while(!window.shouldClose()) {
		spdlog::info("App running");
		glfwPollEvents();
		drawFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		break;
	}

	logicalDevice.waitIdle();
}

void Application::drawFrame() {
	logicalDevice.waitForFences(fences, currentFrame, fenceTimeout);
	uint32_t currentBuffer = logicalDevice.acquireNextImageKHR(swapChain, imageAcquiredSemaphores,
	                                                                            currentFrame, fenceTimeout);
	logicalDevice.waitForFences(fences, currentBuffer, fenceTimeout);
	//TODO: Check if needed and possible: imagesInFlight.setFence(std::move(fences.fences[currentFrame]), currentBuffer.value);
	logicalDevice.resetFences(fences, currentFrame);
	logicalDevice.submitToGraphicsQueue(imageAcquiredSemaphores, renderFinishedSemaphores, commandBuffer, fences, currentFrame);
	logicalDevice.presentKHR(renderFinishedSemaphores, currentFrame, swapChain, currentBuffer);
	currentFrame = (currentFrame+1)%MAX_FRAMES_IN_FLIGHT;
}
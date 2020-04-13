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
drawFence(logicalDevice.createFence()),
imageAcquiredSemaphore(logicalDevice.createSemaphore(magmatic::SemaphoreType::ImageAvailableSemaphore)) {
	spdlog::info("Application constructor called and finished work");
}

void Application::run() {
	while(!window.shouldClose()) {
		spdlog::info("App running");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}
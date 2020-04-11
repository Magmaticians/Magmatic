#include <iostream>
#include <spdlog/spdlog.h>
#include "Application.hpp"

Application::Application():window(magmatic::Window(DEFAULT_NAME)),
instance(magmatic::Instance(DEFAULT_NAME, window.getRequiredExtensions())),
surface(instance.createSurface(window)),
physicalDevice(magmatic::PhysicalDevice(instance.enumeratePhysicalDevices()[1])),
logicalDevice(magmatic::LogicalDevice(physicalDevice, surface)),
vertShader(logicalDevice.createShader("./examples/first_test/vert.spv", vk::ShaderStageFlagBits::eVertex)),
fragShader(logicalDevice.createShader("./examples/first_test/frag.spv", vk::ShaderStageFlagBits::eFragment)),
swapChain(logicalDevice.createSwapchain(surface, window.getSize().first, window.getSize().second)),
renderPass(logicalDevice.createRenderPass(surface)),
pipeline(logicalDevice.createPipeline(swapChain.extent.width, swapChain.extent.height, {&vertShader, &fragShader}, renderPass)) {
	spdlog::info("Application constructor called and finished work");
}

void Application::run() {
	spdlog::info("App running");
}
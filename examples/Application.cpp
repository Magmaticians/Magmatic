#include <iostream>
#include "Application.hpp"

Application::Application():window(magmatic::Window(DEFAULT_NAME)),
instance(magmatic::Instance(DEFAULT_NAME, window.getRequiredExtensions())),
surface(instance.createSurface(window)),
physicalDevice(magmatic::PhysicalDevice(instance.enumeratePhysicalDevices()[0])),
logicalDevice(magmatic::LogicalDevice(physicalDevice, surface)),
swapChain(logicalDevice.createSwapchain(surface, window.getSize().first, window.getSize().second)) {
    std::cout<<"Application constructor called"<<std::endl;
}

void Application::run() {
    std::cout<<"App running"<<std::endl;
}
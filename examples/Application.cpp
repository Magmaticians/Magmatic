#include <iostream>
#include "Application.hpp"

Application::Application():window(magmatic::Window(DEFAULT_NAME)), instance(magmatic::Instance(DEFAULT_NAME, window.getRequiredExtensions())), surface(instance.createSurface(window)) {
    std::cout<<"Initializing window"<<std::endl;
    //window = magmatic::Window(DEFAULT_NAME);
    std::cout<<"Initializing Vulkan"<<std::endl;
}

void Application::run() {
    std::cout<<"App running"<<std::endl;
}
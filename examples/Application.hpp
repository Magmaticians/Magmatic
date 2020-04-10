#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include "Window.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};
    magmatic::Window window;
    magmatic::Instance instance;
    magmatic::Surface surface;
    magmatic::PhysicalDevice physicalDevice;
    magmatic::LogicalDevice logicalDevice;
    magmatic::SwapChain swapChain;
    magmatic::RenderPass renderPass;

public:
    Application();

    void run();
};


#endif //MAGMATIC_APPLICATION_H

#ifndef MAGMATIC_APPLICATION_H
#define MAGMATIC_APPLICATION_H

#include "Window.hpp"
#include "Instance.hpp"
#include "Surface.hpp"

class Application {
    static constexpr const char* DEFAULT_NAME{"Test application"};
    magmatic::Window window;
    magmatic::Instance instance;
    magmatic::Surface surface;
public:
    Application();

    void run();
};


#endif //MAGMATIC_APPLICATION_H

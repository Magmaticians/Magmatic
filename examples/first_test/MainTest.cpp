#include <iostream>
#include <spdlog/spdlog.h>
#include "Application.hpp"

int main() {
	spdlog::info("Starting first test");

    Application application;
    application.run();
    return 0;
}
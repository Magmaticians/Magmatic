#include <spdlog/spdlog.h>
#include "Application.hpp"

int main() {
	spdlog::info("Starting first test");

    Application application("hourglass");
    application.run();
    return 0;
}
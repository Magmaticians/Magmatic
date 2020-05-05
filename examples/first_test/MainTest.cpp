#include <spdlog/spdlog.h>
#include "Application.hpp"


int main() {
#if !defined(NDEBUG)
	spdlog::set_level(spdlog::level::debug);
#endif
	spdlog::info("Starting first test");

    Application application("hourglass");
    application.run();
    return 0;
}

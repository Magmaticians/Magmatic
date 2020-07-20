#include "App.hpp"
#include "EndSystem.hpp"
#include <chrono>
#include <spdlog/spdlog.h>

App::App()
{
	ecs_.registerSystem<EndSystem>();
	ecs_.eventRelay().subscribe<ShouldQuit>(
			[this](const auto& event)
			{
				should_quit_ = true;
			});
}

void App::start()
{
	loop();
}

void App::loop()
{
	auto last_frame = std::chrono::high_resolution_clock::now();
	while(!should_quit_)
	{
		const auto frame_start = std::chrono::high_resolution_clock::now();
		const auto delta = std::chrono::duration_cast<std::chrono::microseconds>(frame_start - last_frame);
		spdlog::debug("Frame delta: {} microseconds", delta.count());
		spdlog::debug("Started new frame");
		ecs_.update(delta);
		spdlog::debug("Ended new frame");
		last_frame = frame_start;
	}
}

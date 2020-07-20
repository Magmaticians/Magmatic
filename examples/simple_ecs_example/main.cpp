#include "App.hpp"
#include <spdlog/spdlog.h>


int main(int argc, char** argv)
{
	spdlog::set_level(spdlog::level::debug);
	App app;
	app.start();
}
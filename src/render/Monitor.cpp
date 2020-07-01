#include "render/Monitor.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

magmatic::render::Monitor magmatic::render::Monitor::getPrimary()
{
	const auto raw_handle = glfwGetPrimaryMonitor();
	if(raw_handle == nullptr)
	{
		spdlog::error("Magmatic: Failed to get primary monitor");
		throw std::runtime_error("Failed to query monitor");
	}

	return magmatic::render::Monitor(raw_handle);
}

std::vector<magmatic::render::Monitor> magmatic::render::Monitor::getAll()
{
	int monitor_count = 0;
	auto monitor_handles = glfwGetMonitors(&monitor_count);

	if(monitor_handles == nullptr)
	{
		spdlog::error("Magmatic: Failed to get all monitors");
		throw std::runtime_error("Failed to query monitors");
	}

	std::vector<Monitor> monitors;
	monitors.reserve(monitor_count);

	std::transform(monitor_handles, monitor_handles + monitor_count, std::back_inserter(monitors),
	               [](const auto monitor) {
		               return Monitor(monitor);
	               });
	return monitors;
}

magmatic::render::Monitor::MonitorPosition magmatic::render::Monitor::position() const noexcept
{
	int x = 0;
	int y = 0;
	glfwGetMonitorPos(monitor_handle_, &x, &y);
	return MonitorPosition{x, y};
}

magmatic::render::Monitor::MonitorSize magmatic::render::Monitor::physicalSize() const noexcept
{
	int width = 0;
	int height = 0;
	glfwGetMonitorPhysicalSize(monitor_handle_, &width, &height);
	return MonitorSize{width, height};
}

magmatic::render::Monitor::MonitorWorkarea magmatic::render::Monitor::workarea() const noexcept
{
	int x;
	int y;
	int width;
	int height;
	glfwGetMonitorWorkarea(monitor_handle_, &x, &y, &width, &height);
	return std::make_pair(MonitorPosition{x, y}, MonitorSize{width, height});
}

std::string magmatic::render::Monitor::name() const noexcept
{
	const auto window_name = glfwGetMonitorName(monitor_handle_);
	return std::string(window_name);
}

magmatic::render::Monitor::MonitorScale magmatic::render::Monitor::scale() const noexcept
{
	float x;
	float y;
	glfwGetMonitorContentScale(monitor_handle_, &x, &y);
	return MonitorScale{x, y};
}

const GLFWmonitor *magmatic::render::Monitor::handle() const noexcept
{
	return monitor_handle_;
}

magmatic::render::Monitor::VideoMode magmatic::render::Monitor::active_video_mode() const noexcept
{
	const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor_handle_);
	return VideoMode{vid_mode->width, vid_mode->height, vid_mode->refreshRate};
}

std::vector<magmatic::render::Monitor::VideoMode> magmatic::render::Monitor::video_modes() const noexcept
{

	int mode_count = 0;
	const GLFWvidmode* vid_mode = glfwGetVideoModes(monitor_handle_, &mode_count);

	std::vector<VideoMode> modes;
	modes.reserve(mode_count);

	std::transform(vid_mode, vid_mode+mode_count, std::back_inserter(modes),
			[](const GLFWvidmode& mode)
			{
				return VideoMode{mode.width, mode.height, mode.refreshRate};
			}
			);
	return modes;
}

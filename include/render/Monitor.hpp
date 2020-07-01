#ifndef MAGMATIC_MONITOR_HPP
#define MAGMATIC_MONITOR_HPP

#include <vector>
#include <string>

typedef struct GLFWmonitor GLFWmonitor;

namespace magmatic::render
{
	class Monitor
	{
	public:
		static Monitor getPrimary();
		static std::vector<Monitor> getAll();

		struct MonitorPosition
		{
			int x;
			int y;
		};

		struct MonitorSize
		{
			int width;
			int height;
		};

		struct MonitorScale
		{
			float x;
			float y;
		};

		using MonitorWorkarea = std::pair<MonitorPosition, MonitorSize>;

		[[nodiscard]] MonitorPosition position() const noexcept;
		[[nodiscard]] MonitorSize physicalSize() const noexcept;
		[[nodiscard]] MonitorWorkarea workarea() const noexcept;
		[[nodiscard]] MonitorScale scale() const noexcept;
		[[nodiscard]] std::string name() const noexcept;

	private:
		GLFWmonitor* monitor_handle_;

		explicit Monitor(GLFWmonitor* handle): monitor_handle_(handle) {};
	};
}

#endif //MAGMATIC_MONITOR_HPP

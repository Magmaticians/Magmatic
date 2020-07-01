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

		using MonitorWorkarea = std::pair<MonitorPosition, MonitorSize>;

		[[nodiscard]] MonitorPosition getPosition() const noexcept;
		[[nodiscard]] MonitorSize getPhysicalSize() const noexcept;
		[[nodiscard]] MonitorWorkarea getWorkarea() const noexcept;
		[[nodiscard]] std::string getName() const noexcept;

	private:
		GLFWmonitor* monitor_handle_;

		explicit Monitor(GLFWmonitor* handle): monitor_handle_(handle) {};
	};
}

#endif //MAGMATIC_MONITOR_HPP

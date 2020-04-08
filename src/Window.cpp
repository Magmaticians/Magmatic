#include "Window.hpp"
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace {
	void errorCallback(int error, const char* description) {
		spdlog::error("GLFW error: {} Error code: {}", description, error);
	}
}

magmatic::Window::Window(int width, int height, const std::string& name)
{
	glfwSetErrorCallback(errorCallback);

	if(glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("GLFW: Failed to initialize");
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	const auto temp_handler = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if(temp_handler == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW: Failed to create window");
	}

	window = std::unique_ptr<GLFWwindow, GLFWWindowDeleter> (temp_handler, GLFWWindowDeleter());
}

void magmatic::Window::setName(const std::string& new_name) noexcept
{
	window_name = new_name;
	glfwSetWindowTitle(window.get(), new_name.c_str());
}

void magmatic::Window::setSize(int width, int height)
{
	if(width <=0 || height <= 0)
	{
		throw std::runtime_error("Failed to resize window. Invalid size!");
	}

	glfwSetWindowSize(window.get(), width, height);
}

void magmatic::Window::hide() noexcept
{
	glfwHideWindow(window.get());
}

void magmatic::Window::minimize() noexcept
{
	glfwIconifyWindow(window.get());
}

void magmatic::Window::show() noexcept
{
	glfwShowWindow(window.get());
}

void magmatic::Window::maximize() noexcept
{
	glfwMaximizeWindow(window.get());
}

void magmatic::Window::restore() noexcept
{
	glfwRestoreWindow(window.get());
}

void magmatic::Window::focus() noexcept
{
	glfwFocusWindow(window.get());
}

std::pair<int, int> magmatic::Window::getSize() const
{
	int width, height;
	glfwGetWindowSize(window.get(), &width, &height);

	return std::make_pair(width, height);
}

std::string magmatic::Window::getName() const noexcept
{
	return window_name;
}

magmatic::Window::~Window()
{
	window.reset();
	glfwTerminate();
}

std::vector<std::string> magmatic::Window::getRequiredExtensions() const
{
	uint32_t glfw_extension_count = 0;
	const char ** glfw_extensions;

	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<std::string> extensions(glfw_extension_count);
	std::copy_n(glfw_extensions, glfw_extension_count, std::back_inserter(extensions));

	return extensions;
}

void magmatic::Window::GLFWWindowDeleter::operator()(GLFWwindow* pointer) noexcept
{
	glfwDestroyWindow(pointer);
}



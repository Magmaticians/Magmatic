#include "../include/Window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

magmatic::Window::Window(int width, int height, const std::string& name)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	const auto temp_handler = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if(temp_handler == nullptr)
	{
		throw std::runtime_error("Magmatic: Failed to create window");
	}

	window = std::unique_ptr<GLFWwindow, GLFWWindowDeleter> (temp_handler, GLFWWindowDeleter());
}

void magmatic::Window::setName(const std::string& new_name) noexcept
{
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

void magmatic::Window::GLFWWindowDeleter::operator()(GLFWwindow* pointer) noexcept
{
	glfwDestroyWindow(pointer);
}



#ifndef MAGMATIC_WINDOW_HPP
#define MAGMATIC_WINDOW_HPP

#include <string>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;


namespace magmatic::render
{
	class Window
	{
	private:
		friend class Instance;

		static constexpr const char* DEFAULT_NAME{"Magmatic"};
		static constexpr int DEFAULT_WIDTH{640};
		static constexpr int DEFAULT_HEIGHT{480};

		std::string window_name;

	public:
		explicit Window(
                const std::string& name=DEFAULT_NAME,
				int width=DEFAULT_WIDTH,
				int height=DEFAULT_HEIGHT
		);

		~Window();

		Window(const Window& rhs) = delete;
		Window& operator=(const Window& rhs) = delete;

		Window(Window&& rhs) = default;
		Window& operator=(Window&& rhs) = default;

		void setName(const std::string& new_name) noexcept;
		[[nodiscard]] std::string getName() const noexcept;

		void setSize(int width, int height);
		[[nodiscard]] std::pair<int, int> getSize() const;

		void hide() noexcept;
		void minimize() noexcept;
		void show() noexcept;
		void maximize() noexcept;
		void restore() noexcept;
		void focus() noexcept;
		[[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(this->window.get()); }

		[[nodiscard]] vk::Extent2D getFramebufferSize() const;

		[[nodiscard]] std::vector<std::string> getRequiredExtensions() const;

		struct GLFWWindowDeleter
		{
			void operator()(GLFWwindow* pointer) noexcept;
		};

		[[nodiscard]] const std::unique_ptr<GLFWwindow, GLFWWindowDeleter>& getWindow() const {
			return this->window;
		}
	private:

		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> window;
	};
}


#endif //MAGMATIC_WINDOW_HPP

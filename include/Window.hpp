#ifndef MAGMATIC_WINDOW_HPP
#define MAGMATIC_WINDOW_HPP

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;


namespace magmatic
{
	class Window
	{
	private:
		static constexpr const char* DEFAULT_NAME{"Magmatic"};
		static constexpr int DEFAULT_WIDTH{640};
		static constexpr int DEFAULT_HEIGHT{480};

		std::string window_name;

	public:
		explicit Window(
				int width=DEFAULT_WIDTH,
				int height=DEFAULT_HEIGHT,
				const std::string& name=DEFAULT_NAME
		);

		~Window();

		Window(const Window& rhs) = delete;
		Window& operator=(const Window& rhs) = delete;

		Window(Window&& rhs) = default;
		Window& operator=(Window&& rhs) = default;

		void setName(const std::string& new_name) noexcept;
		std::string getName() const noexcept;

		void setSize(int width, int height);
		std::pair<int, int> getSize() const;

		void hide() noexcept;
		void minimize() noexcept;
		void show() noexcept;
		void maximize() noexcept;
		void restore() noexcept;
		void focus() noexcept;

		std::vector<std::string> getRequiredExtensions() const;

		struct GLFWWindowDeleter
		{
			void operator()(GLFWwindow* pointer) noexcept;
		};

	private:

		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> window;
	};
}


#endif //MAGMATIC_WINDOW_HPP

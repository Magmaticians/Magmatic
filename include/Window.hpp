#ifndef MAGMATIC_WINDOW_HPP
#define MAGMATIC_WINDOW_HPP


#include <string>
#include <memory>

struct GLFWwindow;


namespace magmatic
{
	static const char* const DEFAULT_NAME{"Magmatic"};
	static const int DEFAULT_WIDTH{640};
	static const int DEFAULT_HEIGHT{480};

	class Window
	{
	public:
		explicit Window(
				int width=DEFAULT_WIDTH,
				int height=DEFAULT_HEIGHT,
				const std::string& name=DEFAULT_NAME
		);

		Window(const Window& rhs) = delete;
		Window& operator=(const Window& rhs) = delete;

		void setName(const std::string& new_name) noexcept;
		void setSize(int width, int height);

		void hide() noexcept;
		void minimize() noexcept;
		void show() noexcept;
		void maximize() noexcept;
		void restore() noexcept;
		void focus() noexcept;

		struct GLFWwindowDeleter
		{
			void operator()(GLFWwindow* pointer) noexcept;
		};
	private:
		std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
	};
}


#endif //MAGMATIC_WINDOW_HPP

#ifndef MAGMATIC_CURSOR_HPP
#define MAGMATIC_CURSOR_HPP

#include "Bitmap.hpp"

typedef struct GLFWcursor GLFWcursor;

namespace magmatic::render
{
	class Cursor
	{
	public:
		Cursor(const Bitmap& cursor_image, int hot_x, int hot_y);
		explicit Cursor(int shape);
		~Cursor();

		Cursor(Cursor&&) noexcept = default;
		Cursor& operator=(Cursor&&) noexcept = delete;

		Cursor(const Cursor&) = delete;
		Cursor& operator=(const Cursor&) = delete;

		const GLFWcursor* handle() const noexcept;

		friend class Window;
	private:
		bool custom_ = false;
		GLFWcursor* cursor_;

		static GLFWcursor* createStandardCursor(int shape);
		static GLFWcursor* createCustomCursor(const Bitmap &cursor_image, int hot_x, int hot_y);
	};
}

#endif //MAGMATIC_CURSOR_HPP

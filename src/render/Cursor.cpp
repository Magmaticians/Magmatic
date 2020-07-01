#include "render/Cursor.hpp"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

magmatic::render::Cursor::Cursor(const Bitmap& cursor_image, int hot_x, int hot_y)
: cursor_(createCustomCursor(cursor_image, hot_x, hot_y)), custom_(true)
{}

magmatic::render::Cursor::Cursor(int shape)
: cursor_(createStandardCursor(shape))
{}

magmatic::render::Cursor::~Cursor()
{
	if(custom_)
	{
		glfwDestroyCursor(cursor_);
	}
}

GLFWcursor *magmatic::render::Cursor::createStandardCursor(int shape)
{
	auto cursor = glfwCreateStandardCursor(shape);
	if(cursor == nullptr)
	{
		spdlog::error("Magmatic: Failed to create standard cursor");
		throw std::runtime_error("Failed to create cursor");
	}
	return cursor;
}

GLFWcursor* magmatic::render::Cursor::createCustomCursor(const Bitmap &cursor_image, int hot_x, int hot_y)
{
	const auto size = cursor_image.size();

	unsigned char* pixels = new unsigned char[cursor_image.getDataSize()];
	memcpy(pixels, cursor_image.getPixels(), cursor_image.getDataSize());

	const GLFWimage image
	{
		size.first,
		size.second,
		pixels
	};
	auto cursor = glfwCreateCursor(&image, hot_x, hot_y);

	delete[](pixels);

	if(cursor == nullptr)
	{
		spdlog::error("Magmatic: Failed to create custom cursor");
		throw std::runtime_error("Failed to create cursor");
	}

	return cursor;
}

const GLFWcursor *magmatic::render::Cursor::handle() const noexcept
{
	return cursor_;
}

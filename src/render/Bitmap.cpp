#define STB_IMAGE_IMPLEMENTATION

#include "render/Bitmap.hpp"
#include <stb/stb_image.h>
#include <spdlog/spdlog.h>


magmatic::render::Bitmap::Bitmap(const std::filesystem::path& file_path)
{
	stbi_uc* temp_pixels = stbi_load(file_path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if(!temp_pixels)
	{
		spdlog::error("Magmatic: Failed to load image from file: {}", file_path.string());
		throw std::runtime_error("Failed to load image");
	}
	pixels = std::unique_ptr<stbi_uc, stbi_ucDeleter>(temp_pixels);
}

std::pair<int, int> magmatic::render::Bitmap::size() const noexcept
{
	return std::make_pair(width, height);
}

int magmatic::render::Bitmap::getChannel() const noexcept
{
	return channels;
}

const std::unique_ptr<stbi_uc, magmatic::render::Bitmap::stbi_ucDeleter>& magmatic::render::Bitmap::getPixels() const noexcept
{
	return pixels;
}

size_t magmatic::render::Bitmap::getDataSize() const noexcept
{
	return width * height * channels;
}

void magmatic::render::Bitmap::stbi_ucDeleter::operator()(stbi_uc* pointer) noexcept
{
	stbi_image_free(pointer);
}

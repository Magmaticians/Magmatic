#define STB_IMAGE_IMPLEMENTATION

#include "render/Bitmap.hpp"
#include <stb/stb_image.h>
#include <spdlog/spdlog.h>


magmatic::render::Bitmap::Bitmap(const std::filesystem::path& file_path)
{
	stbi_uc* temp_pixels = stbi_load(file_path.string().c_str(), &width_, &height_, &channels_, STBI_rgb_alpha);

	if(!temp_pixels)
	{
		spdlog::error("Magmatic: Failed to load image from file: {}", file_path.string());
		throw std::runtime_error("Failed to load image");
	}
	pixels_.resize(getDataSize());
	memcpy(pixels_.data(), temp_pixels, getDataSize());
	stbi_image_free(temp_pixels);
}

std::pair<int, int> magmatic::render::Bitmap::size() const noexcept
{
	return std::make_pair(width_, height_);
}

int magmatic::render::Bitmap::getChannel() const noexcept
{
	return channels_;
}

const uint8_t* magmatic::render::Bitmap::getPixels() const noexcept
{
	return pixels_.data();
}

size_t magmatic::render::Bitmap::getDataSize() const noexcept
{
	return width_ * height_ * 4;
}

magmatic::render::Bitmap::Bitmap(
		const std::vector<uint8_t>& pixels, int width, int height, int channels
) noexcept
: width_(width), height_(height), channels_(channels)
{
	pixels_.resize(getDataSize());
	if(channels == 4)
	{
		memcpy(pixels_.data(), pixels.data(), getDataSize());
	}
	else
	{
		auto rgb = pixels.begin();
		auto rgba = pixels_.begin();
		for(size_t i = 0; i < width * height; ++i)
		{
			for(size_t j = 0; j < 3; ++j)
			{
				*(rgba++) = *(rgb++);
			}
			*(rgba++) = 0;
		}
	}
}

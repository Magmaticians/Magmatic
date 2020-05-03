#ifndef MAGMATIC_BITMAP_HPP
#define MAGMATIC_BITMAP_HPP

#include <filesystem>
#include <memory>
#include <vulkan/vulkan.hpp>

typedef unsigned char stbi_uc;

namespace magmatic::render
{
	class Bitmap
	{
	private:
		int width_{0};
		int height_{0};
		int channels_ {0};
		std::vector<uint8_t> pixels_;

	public:
		Bitmap() = default;
		explicit Bitmap(const std::filesystem::path& file_path);
		explicit Bitmap(const std::vector<uint8_t>& pixels, int width, int height, int channels) noexcept;

		Bitmap(const Bitmap&) = delete;
		Bitmap& operator=(const Bitmap&) = delete;

		Bitmap(const Bitmap&& rhs) noexcept;
		Bitmap&operator=(Bitmap&& rhs) noexcept;

		[[nodiscard]] std::pair<int, int> size() const noexcept;
		[[nodiscard]] int getChannel() const noexcept;
		[[nodiscard]] const uint8_t* getPixels() const noexcept;
		[[nodiscard]] size_t getDataSize() const noexcept;
	};
}


#endif //MAGMATIC_BITMAP_HPP

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
	public:
		struct stbi_ucDeleter
		{
			void operator()(stbi_uc* pointer) noexcept;
		};

	private:
		int width;
		int height;
		int channels;
		std::unique_ptr<stbi_uc, stbi_ucDeleter> pixels;

	public:

		explicit Bitmap(const std::filesystem::path& file_path);

		Bitmap(const Bitmap&) = delete;
		Bitmap& operator=(const Bitmap&) = delete;

		[[nodiscard]] std::pair<int, int> size() const noexcept;
		[[nodiscard]] int getChannel() const noexcept;
		[[nodiscard]] const std::unique_ptr<stbi_uc, stbi_ucDeleter>& getPixels() const noexcept;
		[[nodiscard]] size_t getDataSize() const noexcept;
	};
}


#endif //MAGMATIC_BITMAP_HPP

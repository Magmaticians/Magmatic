#ifndef MAGMATIC_IMAGE_HPP
#define MAGMATIC_IMAGE_HPP

#include <filesystem>
#include <memory>


typedef unsigned char stbi_uc;

namespace magmatic::render
{
	class Image
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

		explicit Image(const std::filesystem::path& file_path);

		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		[[nodiscard]] std::pair<int, int> size() const noexcept;
		[[nodiscard]] int getChannel() const noexcept;
		[[nodiscard]] const std::unique_ptr<stbi_uc, stbi_ucDeleter>& getPixels() const noexcept;
		[[nodiscard]] size_t getDataSize() const noexcept;
	};
}


#endif //MAGMATIC_IMAGE_HPP

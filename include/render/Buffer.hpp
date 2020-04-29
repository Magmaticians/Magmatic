#ifndef MAGMATIC_BUFFER_HPP
#define MAGMATIC_BUFFER_HPP

#include "render/LogicalDevice.hpp"
#include "CommandPool.hpp"
#include <vulkan/vulkan.hpp>


namespace magmatic::render
{
	class Buffer {
	protected:
		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;

	public:
		Buffer(
				const LogicalDevice& l_device,
				vk::DeviceSize size,
				vk::BufferUsageFlags usageFlags,
				vk::MemoryPropertyFlags memoryFlags
				);

		Buffer(const Buffer&) = delete;
		Buffer& operator=(Buffer&) = delete;

		Buffer(Buffer&& rhs) noexcept;
		Buffer& operator=(Buffer&& rhs) noexcept;

		template <typename T>
		static Buffer createStaging(const LogicalDevice& l_device,const T* data_to_copy, size_t data_size);

		[[nodiscard]] const vk::UniqueBuffer& getBuffer() const noexcept;
		[[nodiscard]] const vk::UniqueDeviceMemory& getMemory() const noexcept;

		static void copyBuffer(
				const vk::UniqueBuffer& src,
				const vk::UniqueBuffer& dst,
				vk::DeviceSize size,
				const CommandPool& pool
		);

		static void copyBufferToImage(
				const vk::UniqueBuffer& src,
				const vk::UniqueImage& dst,
				uint32_t width, uint32_t height,
				const CommandPool& pool
		);

	};
}

template<typename T>
magmatic::render::Buffer magmatic::render::Buffer::createStaging(
		const LogicalDevice& l_device,
		const T* data_to_copy,
		size_t data_size
		)
{
	Buffer staging {
		l_device,
		data_size,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	};

	void* buffer_mem;
	l_device.getHandle()->mapMemory(staging.getMemory().get(), 0, data_size, vk::MemoryMapFlags(), &buffer_mem);
	memcpy(buffer_mem, data_to_copy, data_size);
	l_device.getHandle()->unmapMemory(staging.getMemory().get());

	return std::move(staging);
}

#endif //MAGMATIC_BUFFER_HPP

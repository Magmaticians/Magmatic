#ifndef MAGMATIC_UNIFORMBUFFER_HPP
#define MAGMATIC_UNIFORMBUFFER_HPP

#include <render/Buffer.hpp>
#include <render/LogicalDevice.hpp>
#include <render/CommandPool.hpp>

namespace magmatic::render
{
	template<typename T>
	class UniformBuffer : public Buffer
	{
	private:
		size_t indices_count;
		constexpr static vk::BufferUsageFlags buffer_usage_flags =
				vk::BufferUsageFlagBits::eUniformBuffer;

		constexpr static vk::MemoryPropertyFlags memory_property =
				vk::MemoryPropertyFlagBits::eHostVisible
				| vk::MemoryPropertyFlagBits::eHostCoherent;

	public:
		UniformBuffer(
				const LogicalDevice& l_device,
				const CommandPool& pool
		)
		:Buffer(l_device, sizeof(T), buffer_usage_flags, memory_property) {}

		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(UniformBuffer&) = delete;

		UniformBuffer(UniformBuffer&& rhs) noexcept: Buffer(std::move(rhs)) {}
		UniformBuffer& operator=(UniformBuffer&& rhs) noexcept {
			Buffer::operator=(std::move(rhs));
			return *this;
		}

		void copyMemory(size_t size, const T& dataToCopy);
	};
}

template<typename T>
void magmatic::render::UniformBuffer<T>::copyMemory(size_t size, const T& dataToCopy) {
	const auto& handle = buffer.getOwner();
	void* data;
	handle.mapMemory(memory.get(), 0, size, vk::MemoryMapFlags(), &data);
	memcpy(data, &dataToCopy, size);
	handle.unmapMemory(memory.get());
}

#endif //MAGMATIC_UNIFORMBUFFER_HPP

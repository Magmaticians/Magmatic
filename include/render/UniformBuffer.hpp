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

		[[nodiscard]] static std::vector<UniformBuffer> createUniformBuffers(size_t count, const LogicalDevice& l_device,
		                                                       const CommandPool& pool);
		[[nodiscard]] static std::vector<std::unique_ptr<UniformBuffer>> createUniformBuffersUnique(size_t count, const LogicalDevice& l_device,
		                                                                     const CommandPool& pool);

		static void reCreateUniformBuffers(std::vector<UniformBuffer>& oldBuffers,
				const LogicalDevice& l_device, const CommandPool& pool);
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

template<typename T>
std::vector<magmatic::render::UniformBuffer<T>> magmatic::render::UniformBuffer<T>::createUniformBuffers(size_t count, const LogicalDevice& l_device,
                                                                     const CommandPool& pool) {
	std::vector<UniformBuffer<T>> res;
	res.reserve(count);
	for(size_t i = 0; i < count; ++i) {
		res.emplace_back(UniformBuffer<T>(l_device, pool));
	}
	return res;
}
template<typename T>
std::vector<std::unique_ptr<magmatic::render::UniformBuffer<T>>> magmatic::render::UniformBuffer<T>::createUniformBuffersUnique(size_t count, const LogicalDevice& l_device,
                                                                                                         const CommandPool& pool) {
	std::vector<std::unique_ptr<UniformBuffer<T>>> res;
	res.reserve(count);
	for(size_t i = 0; i < count; ++i) {
		res.emplace_back(std::make_unique<UniformBuffer<T>>(l_device, pool));
	}
	return res;
}

template<typename T>
void magmatic::render::UniformBuffer<T>::reCreateUniformBuffers(std::vector<UniformBuffer<T>>& oldBuffers,
		const LogicalDevice& l_device, const CommandPool& pool) {
	for(size_t i = 0; i < oldBuffers.size(); ++i) {
		oldBuffers[i] = std::move(UniformBuffer<T>(l_device, pool));
	}
}

#endif //MAGMATIC_UNIFORMBUFFER_HPP

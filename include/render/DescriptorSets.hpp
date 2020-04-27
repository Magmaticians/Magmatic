#ifndef MAGMATIC_DESCRIPTORSETS_HPP
#define MAGMATIC_DESCRIPTORSETS_HPP

#include <vulkan/vulkan.hpp>

namespace magmatic::render
{
	class DescriptorSets
	{
		friend class LogicalDevice;

		explicit DescriptorSets(std::vector<vk::DescriptorSet> sets, vk::UniqueDescriptorPool descriptorPool) : sets(std::move(sets)), descriptorPool(std::move(descriptorPool)) {};
	public:
		vk::UniqueDescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> sets;

		DescriptorSets(DescriptorSets&) = delete;
		DescriptorSets& operator=(DescriptorSets&) = delete;

		DescriptorSets(DescriptorSets&& rhs) noexcept : sets(std::move(rhs.sets)), descriptorPool(std::move(rhs.descriptorPool)) {};
		DescriptorSets& operator=(DescriptorSets&& rhs) noexcept
		{
			sets = std::move(rhs.sets);
			descriptorPool = std::move(rhs.descriptorPool);
			return *this;
		};

		[[nodiscard]] size_t size() const { return sets.size(); }
	};
}

#endif //MAGMATIC_DESCRIPTORSETS_HPP

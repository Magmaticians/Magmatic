#ifndef MAGMATIC_DESCRIPTORSETS_HPP
#define MAGMATIC_DESCRIPTORSETS_HPP

#include <vulkan/vulkan.hpp>
#include "LogicalDevice.hpp"
#include "DescriptorWriteUpdate.hpp"

namespace magmatic::render
{
	class DescriptorSets
	{

		vk::UniqueDescriptorSetLayout descriptor_set_layout;
	public:
		vk::UniqueDescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> sets;

		DescriptorSets(const LogicalDevice& l_device, const std::vector<vk::DescriptorSetLayoutBinding>& bindings, size_t count, std::vector<vk::DescriptorType> types);

		DescriptorSets(DescriptorSets&) = delete;
		DescriptorSets& operator=(DescriptorSets&) = delete;

		DescriptorSets(DescriptorSets&& rhs) noexcept : sets(std::move(rhs.sets)), descriptorPool(std::move(rhs.descriptorPool)) {};
		DescriptorSets& operator=(DescriptorSets&& rhs) noexcept;

		void updateDescriptorSet(size_t index,const std::vector<DescriptorWriteUpdate>& write_info) const;

		[[nodiscard]] size_t size() const { return sets.size(); }
		[[nodiscard]] const vk::UniqueDescriptorSetLayout& getDescriptorSetLayout() const { return descriptor_set_layout; }
	};
}

#endif //MAGMATIC_DESCRIPTORSETS_HPP

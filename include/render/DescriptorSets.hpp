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
		vk::UniqueDescriptorPool descriptorPool;
		std::vector<vk::UniqueDescriptorSet> sets;
	public:

		DescriptorSets(const LogicalDevice& l_device, const std::vector<vk::DescriptorSetLayoutBinding>& bindings, size_t count);

		DescriptorSets(const DescriptorSets&) = delete;
		DescriptorSets& operator=(const DescriptorSets&) = delete;

		DescriptorSets(DescriptorSets&& rhs) noexcept = default;
		DescriptorSets& operator=(DescriptorSets&& rhs) noexcept = default;

		~DescriptorSets() = default;

		void updateDescriptorSet(size_t index,const std::vector<DescriptorWriteUpdate>& write_info) const;

		[[nodiscard]] size_t size() const { return sets.size(); }
		[[nodiscard]] const vk::UniqueDescriptorSetLayout& getDescriptorSetLayout() const { return descriptor_set_layout; }

		const vk::UniqueDescriptorSet& operator[](size_t index) const {
			return sets.at(index);
		}
	};
}

#endif //MAGMATIC_DESCRIPTORSETS_HPP

#include "render/DescriptorSets.hpp"

magmatic::render::DescriptorSets::DescriptorSets(const LogicalDevice& l_device, const std::vector<vk::DescriptorSetLayoutBinding>& bindings, size_t count) {
	const auto& handle = l_device.getHandle();

	auto size = static_cast<uint32_t>(count);
	std::vector<vk::DescriptorPoolSize> sizes;
	sizes.reserve(bindings.size());


	vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(
			vk::DescriptorSetLayoutCreateFlags(),
			bindings.size(),
			bindings.data()
	);
	descriptor_set_layout = handle->createDescriptorSetLayoutUnique(descriptor_set_layout_create_info);

	std::transform(
			bindings.begin(),
			bindings.end(),
			std::back_inserter(sizes),
			[size](const vk::DescriptorSetLayoutBinding& binding)
			{
				return vk::DescriptorPoolSize
						{
								binding.descriptorType,
								size * binding.descriptorCount
						};
			}
	);

	vk::DescriptorPoolCreateInfo poolInfo(
			vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			size,
			sizes.size(),
			sizes.data()
	);
	descriptorPool = handle->createDescriptorPoolUnique(poolInfo);

	std::vector<vk::DescriptorSetLayout> layouts(size, descriptor_set_layout.get());
	vk::DescriptorSetAllocateInfo allocInfo(
			descriptorPool.get(),
			size,
			layouts.data()
	);
	sets = std::move(handle->allocateDescriptorSetsUnique(allocInfo));
}

magmatic::render::DescriptorSets& magmatic::render::DescriptorSets::operator=(DescriptorSets&& rhs) noexcept {
	sets = std::move(rhs.sets);
	descriptorPool = std::move(rhs.descriptorPool);
	return *this;
}

void magmatic::render::DescriptorSets::updateDescriptorSet(size_t index, const std::vector<DescriptorWriteUpdate>& write_info) const {
	const auto& handle = descriptor_set_layout.getOwner();
	std::vector<vk::WriteDescriptorSet> write_ops;
	write_ops.reserve(write_info.size());
	std::transform(
			write_info.begin(),
			write_info.end(),
			std::back_inserter(write_ops),
			[this, index](const auto& info){return info.toWriteInfo(this->sets[index].get());}
	);
	handle.updateDescriptorSets(write_ops.size(), write_ops.data(), 0, nullptr);
}

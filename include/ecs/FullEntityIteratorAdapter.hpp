#ifndef MAGMATIC_FULLENTITYITERATORADAPTER_HPP
#define MAGMATIC_FULLENTITYITERATORADAPTER_HPP

#include "FullEntity.hpp"
#include <iterator>
#include <concepts>

namespace magmatic::ecs
{
	template<typename T>
	concept EntityIterator =
			std::input_iterator<T>
			&& std::equality_comparable<T>
			&& std::same_as<typename T::self_type, typename FullEntity::EntityID>;

	template<EntityIterator T>
	class FullEntityIteratorAdapter
	{
	public:
		using self_type = FullEntityIteratorAdapter;
		using value_type = FullEntity;
		using reference = FullEntity;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::input_iterator_tag;

		FullEntityIteratorAdapter(): internal_iter{} {};
		FullEntityIteratorAdapter(T iter, ComponentManager& component_manager)
		:internal_iter(iter), manager(component_manager) {};

		value_type operator*() const
		{
			return FullEntity(*internal_iter, manager);
		}
		bool operator==(const FullEntityIteratorAdapter& other) const
		{
			return &manager == &other.manager && internal_iter == other.internal_iter;
		}
		bool operator!=(const FullEntityIteratorAdapter& other) const
		{
			return !(*this == other);
		}
		self_type& operator++()
		{
			++internal_iter;
			return *this;
		}
		self_type operator++(int)
		{
			auto ret = *this;
			++internal_iter;
			return ret;
		}
	private:
		T internal_iter;
		ComponentManager& manager;
	};
}

#endif //MAGMATIC_FULLENTITYITERATORADAPTER_HPP

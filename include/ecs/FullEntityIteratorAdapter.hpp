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

		FullEntityIteratorAdapter(): internal_iter_{} {};
		FullEntityIteratorAdapter(T iter, EntityManager& entity_manager, ComponentManager& component_manager)
		: internal_iter_(iter), entity_manager_(entity_manager), component_manager_(component_manager) {};

		value_type operator*() const
		{
			return FullEntity(*internal_iter_, <#initializer#>, component_manager_);
		}
		bool operator==(const FullEntityIteratorAdapter& other) const
		{
			return &component_manager_ == &other.component_manager_ && internal_iter_ == other.internal_iter_;
		}
		bool operator!=(const FullEntityIteratorAdapter& other) const
		{
			return !(*this == other);
		}
		self_type& operator++()
		{
			++internal_iter_;
			return *this;
		}
		self_type operator++(int)
		{
			auto ret = *this;
			++internal_iter_;
			return ret;
		}
	private:
		T internal_iter_;

		EntityManager& entity_manager_;
		ComponentManager& component_manager_;
	};
}

#endif //MAGMATIC_FULLENTITYITERATORADAPTER_HPP

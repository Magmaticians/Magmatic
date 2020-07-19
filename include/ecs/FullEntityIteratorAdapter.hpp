#ifndef MAGMATIC_FULLENTITYITERATORADAPTER_HPP
#define MAGMATIC_FULLENTITYITERATORADAPTER_HPP

#include "FullEntity.hpp"
#include "ECS.hpp"
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

		FullEntityIteratorAdapter(): internal_iter_{} {} noexcept;
		FullEntityIteratorAdapter(T iter, ECS& ecs_handle) noexcept
		: internal_iter_(iter), ecs_handle_(ecs_handle) {};

		value_type operator*() const
		{
			return FullEntity(*internal_iter_, ecs_handle_);
		}
		bool operator==(const FullEntityIteratorAdapter& other) const
		{
			return &ecs_handle_ == &other.ecs_handle_ && internal_iter_ == other.internal_iter_;
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

		ECS& ecs_handle_;
	};
}

#endif //MAGMATIC_FULLENTITYITERATORADAPTER_HPP

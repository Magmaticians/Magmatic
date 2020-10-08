#ifndef MAGMATIC_ENTITYMANAGER_HPP
#define MAGMATIC_ENTITYMANAGER_HPP

#include <queue>
#include <bitset>
#include <vector>
#include <unordered_map>
#include "ECSConfig.hpp"
#include "ECSTypes.hpp"

namespace magmatic::ecs
{

	class EntityManager
	{
	public:
		class Iterator;


		using self_type = EntityManager;
		using iterator = Iterator;
		using const_iterator = Iterator;

		explicit EntityManager();

		[[nodiscard]] entity_id_t addEntity();
		void removeEntity(entity_id_t id);

		void setComponentMask(entity_id_t id, components_mask_t mask);
		[[nodiscard]] const components_mask_t& getComponentMask(entity_id_t id) const;

		[[nodiscard]] bool entityExists(entity_id_t id) const noexcept;

		[[nodiscard]] std::size_t size() const noexcept;
		[[nodiscard]] std::size_t capacity() const noexcept;

		[[nodiscard]] iterator begin() const noexcept;
		[[nodiscard]] iterator cbegin() const noexcept;

		[[nodiscard]] iterator end() const noexcept;
		[[nodiscard]] iterator cend() const noexcept;

	private:
		struct EntityInfo
		{
			bool exist = false;
			components_mask_t mask;
		};

		std::queue<entity_id_t> free_IDs;
		std::vector<EntityInfo> entities;

		std::size_t last_free_id = 0;
		std::size_t size_ = 0;

		void checkEntityId(entity_id_t id) const;

		template<typename Derived>
		class BaseIterator
		{
			std::size_t index_;
			const EntityManager* manager_;

		public:
			using self_type = BaseIterator;
			using value_type = entity_id_t;
			using reference = entity_id_t;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::input_iterator_tag;

			BaseIterator(): index_{0}, manager_{nullptr} {};

			value_type operator*() const
			{
				return index_;
			}
			bool operator==(const BaseIterator& other) const
			{
				return manager_ == other.manager_ && index_ == other.index_;
			}
			bool operator!=(const BaseIterator& other) const
			{
				return !(*this == other);
			}
			self_type& operator++()
			{
				++index_;
				next();
				return *this;
			}
			self_type operator++(int)
			{
				auto ret = *this;
				++index_;
				next();
				return ret;
			}
		protected:
			BaseIterator(const EntityManager* manager, std::size_t index)
			: index_(index), manager_(manager) {}

			inline bool matching()
			{
				return manager_->entityExists(index_) && static_cast<Derived*>(this)->valid(manager_->entities[index_]);
			}
			void next()
			{
				while (index_ < manager_->capacity() && !matching())
				{
					++index_;
				}
			}
		};

	public:
		class Iterator : public BaseIterator<Iterator>
		{
		public:
			Iterator()
			: BaseIterator<Iterator>(nullptr, 0) {}

			[[nodiscard]] bool valid(const EntityInfo&) const
			{
				return true;
			}
		protected:
			Iterator(const EntityManager* manager, std::size_t index)
			: BaseIterator<Iterator>(manager, index)
			{
				next();
			}

			friend class EntityManager;
		};

		class MaskedView
		{
		public:
			class MaskedIterator;

			using iterator = MaskedIterator;
			using const_iterator = MaskedIterator;

			const EntityManager& manager;
			components_mask_t mask;

			explicit MaskedView(const EntityManager& entity_manager) noexcept
			: manager(entity_manager), mask{} {};

			MaskedView(const EntityManager& entity_manager, const components_mask_t& components_mask) noexcept
			: manager(entity_manager), mask(components_mask) {};

			[[nodiscard]] iterator begin() const noexcept;
			[[nodiscard]] iterator cbegin() const noexcept;

			[[nodiscard]] iterator end() const noexcept;
			[[nodiscard]] iterator cend() const noexcept;

			class MaskedIterator : public BaseIterator<MaskedIterator>
			{
			private:
				components_mask_t mask_{};

			public:
				MaskedIterator()
						: BaseIterator<MaskedIterator>(nullptr, 0), mask_{} {}

				[[nodiscard]] bool valid(const EntityInfo& info) const
				{
					return (info.mask & mask_) == mask_;
				}
			protected:
				MaskedIterator(const EntityManager* manager, const components_mask_t & mask, std::size_t index)
				: BaseIterator<MaskedIterator>(manager, index), mask_(mask)
				{
					next();
				}

				friend class EntityManager;
			};
		};
	};
}

#endif //MAGMATIC_ENTITYMANAGER_HPP

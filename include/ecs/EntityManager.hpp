#ifndef MAGMATIC_ENTITYMANAGER_HPP
#define MAGMATIC_ENTITYMANAGER_HPP

#include <queue>
#include <bitset>
#include <vector>
#include <unordered_map>

namespace magmatic::ecs
{
	constexpr uint8_t COMPONENT_TYPE_COUNT = 10;
	constexpr std::size_t MAX_ENTITIES_COUNT = 2048;

	class EntityManager
	{
	public:
		class Iterator;

		using EntityID = std::size_t;
		using ComponentsMask = std::bitset<COMPONENT_TYPE_COUNT>;

		using self_type = EntityManager;
		using iterator = Iterator;
		using const_iterator = Iterator;

		explicit EntityManager();

		EntityID addEntity();
		void removeEntity(EntityID id);

		void setComponentMask(EntityID id, ComponentsMask mask);
		[[nodiscard]] const ComponentsMask& getComponentMask(EntityID id) const;

		[[nodiscard]] bool entityExists(EntityID id) const;

		[[nodiscard]] std::size_t size() const;
		[[nodiscard]] std::size_t capacity() const;

		[[nodiscard]] iterator begin() const;
		[[nodiscard]] iterator cbegin() const;

		[[nodiscard]] iterator end() const;
		[[nodiscard]] iterator cend() const;

	private:
		struct EntityInfo
		{
			bool exist = false;
			ComponentsMask mask;
		};

		std::queue<EntityID> free_IDs;
		std::vector<EntityInfo> entities;

		std::size_t last_free_id = 0;
		std::size_t size_ = 0;

		void check_entity_id(EntityID id) const;

		template<typename Derived>
		class BaseIterator
		{
			std::size_t index_;
			const EntityManager* manager_;

		public:
			using self_type = BaseIterator;
			using value_type = EntityID;
			using reference = EntityID;
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
			: manager_(manager), index_(index)
			{
				next();
			}

			inline bool matching()
			{
				return manager_->entityExists(index_) && static_cast<Derived*>(this)->valid();
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
			bool valid() const
			{
				return true;
			}
		protected:
			Iterator(const EntityManager* manager, std::size_t index)
			: BaseIterator<Iterator>(manager, index)
			{}

			friend class EntityManager;
		};
	};
}

#endif //MAGMATIC_ENTITYMANAGER_HPP

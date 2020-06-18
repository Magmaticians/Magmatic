#ifndef MAGMATIC_ENTITYMANAGER_HPP
#define MAGMATIC_ENTITYMANAGER_HPP

#include <queue>
#include <bitset>
#include <vector>
#include <unordered_map>

namespace magmatic::ecs
{
	class EntityManager
	{
	public:
		constexpr static uint8_t ComponentTypes = 10;

		using EntityID = std::size_t;
		using ComponentsMask = std::bitset<ComponentTypes>;

		explicit EntityManager(std::size_t capacity);

		EntityID addEntity();
		void removeEntity(EntityID id);

		void setComponentMask(EntityID id, ComponentsMask mask);
		[[nodiscard]] const ComponentsMask& getComponentMask(EntityID id) const;

		[[nodiscard]] bool entityExists(EntityID id) const noexcept;

	private:
		struct EntityInfo
		{
			bool exist = false;
			ComponentsMask mask;
		};

		std::queue<EntityID> free_IDs;
		std::vector<EntityInfo> entities;

		std::size_t last_free_id = 0;

		void check_entity_id(EntityID id) const;
	};
}

#endif //MAGMATIC_ENTITYMANAGER_HPP

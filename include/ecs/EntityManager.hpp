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
		using EntityID = std::size_t;
		using ComponentsMask = std::bitset<COMPONENT_TYPE_COUNT>;

		explicit EntityManager();

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

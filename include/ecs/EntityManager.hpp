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

		using EntityID = uint32_t ;
		using ComponentsMask = std::bitset<ComponentTypes>;

		EntityID addEntity();
		void removeEntity(EntityID id);

		void setComponentMask(EntityID id, ComponentsMask mask);
		[[nodiscard]] const ComponentsMask& getComponentMask(EntityID id) const;

		[[nodiscard]] bool entityExists(EntityID id) const noexcept;



	private:
		std::queue<EntityID> free_IDs;
		std::unordered_map<EntityID, ComponentsMask> componentMasks;

		std::size_t last_free_id = 0;
	};
}

#endif //MAGMATIC_ENTITYMANAGER_HPP

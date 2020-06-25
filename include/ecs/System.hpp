#ifndef MAGMATIC_SYSTEM_H
#define MAGMATIC_SYSTEM_H

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include <set>
#include <chrono>


namespace magmatic::ecs
{
	class System
	{
	public:
		using EntityID = EntityManager::EntityID;
		std::set<EntityID> handled_entities;

		virtual void update(
				const std::chrono::duration<int64_t, std::micro>& delta,
				const ComponentManager& component_manager
				) = 0;

		virtual ~System() = default;
	};
}

#endif //MAGMATIC_SYSTEM_H

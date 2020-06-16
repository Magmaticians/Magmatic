#ifndef MAGMATIC_SYSTEM_H
#define MAGMATIC_SYSTEM_H

#include <set>
#include "EntityManager.hpp"

namespace magmatic::ecs
{
	class System
	{
	public:
		using EntityID = EntityManager::EntityID;
		std::set<EntityID> handled_entities;
	};
}

#endif //MAGMATIC_SYSTEM_H

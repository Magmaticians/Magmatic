#ifndef MAGMATIC_SYSTEM_HPP
#define MAGMATIC_SYSTEM_HPP

#include "EntityManager.hpp"
#include "EventRelay.hpp"
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

		virtual void configure(EventRelay& event_relay) {};

		virtual void update(
				const std::chrono::duration<int64_t, std::micro>& delta,
				ComponentManager& component_manager
				) = 0;

		virtual ~System() = default;
	};
}

#endif //MAGMATIC_SYSTEM_HPP

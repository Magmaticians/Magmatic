#ifndef MAGMATIC_ECS_HPP
#define MAGMATIC_ECS_HPP

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "FullEntity.hpp"
#include "EventRelay.hpp"


namespace magmatic::ecs
{
	class ECS
	{
	public:
		using EntityID = EntityManager::EntityID;

		ECS();
		[[nodiscard]] FullEntity createEntity();
		void removeEntity(EntityID id);

	private:
		EventRelay event_relay_;

		EntityManager entity_manager_;
		ComponentManager component_manager_;
		SystemManager system_manager_;
	};
}

#endif //MAGMATIC_ECS_HPP

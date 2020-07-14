#include "ecs/ECS.hpp"

magmatic::ecs::ECS::ECS()
:system_manager_(component_manager_, event_relay_)
{
}

magmatic::ecs::FullEntity magmatic::ecs::ECS::createEntity()
{
	const auto entity_id = entity_manager_.addEntity();
	system_manager_.updateEntityMask(entity_id, {});

	return FullEntity(entity_id, component_manager_);
}

void magmatic::ecs::ECS::removeEntity(magmatic::ecs::ECS::EntityID id)
{
	system_manager_.entityRemoved(id);
	entity_manager_.removeEntity(id);
}


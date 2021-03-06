#include "ecs/ECS.hpp"
#include "ecs/FullEntity.hpp"

magmatic::ecs::ECS::ECS()
:system_manager_(component_manager_, event_relay_)
{
}

magmatic::ecs::FullEntity magmatic::ecs::ECS::createEntity()
{
	const auto entity_id = entity_manager_.addEntity();
	system_manager_.updateEntityMask(entity_id, {});

	return FullEntity(entity_id, *this);
}

void magmatic::ecs::ECS::removeEntity(magmatic::ecs::ECS::EntityID id)
{
	system_manager_.entityRemoved(id);
	component_manager_.removeEntityComponents(id);
	entity_manager_.removeEntity(id);
}

void magmatic::ecs::ECS::update(const std::chrono::duration<int64_t, std::micro>& delta)
{
	system_manager_.update(delta);
}

bool magmatic::ecs::ECS::entityExists(magmatic::ecs::ECS::EntityID id) const noexcept
{
	return entity_manager_.entityExists(id);
}

magmatic::ecs::EventRelay& magmatic::ecs::ECS::eventRelay() noexcept
{
	return event_relay_;
}

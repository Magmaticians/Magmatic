#include "ecs/ECS.hpp"

#include <utility>
#include "ecs/FullEntity.hpp"

magmatic::ecs::ECS::ECS(thread_pool_ptr thread_pool)
:system_manager_(*this, std::move(thread_pool))
{
}

magmatic::ecs::entity_id_t magmatic::ecs::ECS::createEntity()
{
	const auto entity_id = entity_manager_.addEntity();
	system_manager_.updateEntityMask(entity_id, {});

	return entity_id;
}

void magmatic::ecs::ECS::removeEntity(entity_id_t id)
{
	system_manager_.entityRemoved(id);
	component_manager_.removeEntityComponents(id);
	entity_manager_.removeEntity(id);
}

void magmatic::ecs::ECS::update(const delta_time_t & delta)
{
	system_manager_.update(delta);
}

bool magmatic::ecs::ECS::entityExists(entity_id_t id) const noexcept
{
	return entity_manager_.entityExists(id);
}

magmatic::ecs::EventRelay& magmatic::ecs::ECS::eventRelay() noexcept
{
	return event_relay_;
}

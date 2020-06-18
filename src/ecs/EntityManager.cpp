#include "ecs/EntityManager.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

magmatic::ecs::EntityManager::EntityManager(std::size_t capacity)
{
	entities.resize(capacity);
}

magmatic::ecs::EntityManager::EntityID magmatic::ecs::EntityManager::addEntity()
{
	EntityID new_id;

	if(!free_IDs.empty())
	{
		new_id = free_IDs.front();
		free_IDs.pop();
	}
	else
	{
		new_id = last_free_id;
		++last_free_id;
	}

	assert(new_id < entities.size());

	entities[new_id].mask.reset();
	entities[new_id].exist = true;
	return new_id;
}

void magmatic::ecs::EntityManager::removeEntity(magmatic::ecs::EntityManager::EntityID id)
{
	check_entity_id(id);

	entities[id].exist = false;
	free_IDs.push(id);
}

bool magmatic::ecs::EntityManager::entityExists(EntityID id) const noexcept
{
	return entities[id].exist;
}

void magmatic::ecs::EntityManager::setComponentMask(EntityID id, ComponentsMask mask)
{
	check_entity_id(id);

	entities[id].mask = mask;
}

const magmatic::ecs::EntityManager::ComponentsMask &
magmatic::ecs::EntityManager::getComponentMask(EntityID id) const
{
	check_entity_id(id);
	return entities.at(id).mask;
}

inline void magmatic::ecs::EntityManager::check_entity_id(EntityID id) const
{
	assert(id < entities.size());
	assert(entities[id].exist);
}
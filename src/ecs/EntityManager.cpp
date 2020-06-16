#include "ecs/EntityManager.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>


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
	componentMasks.insert({new_id, {}});
	return new_id;
}

void magmatic::ecs::EntityManager::removeEntity(magmatic::ecs::EntityManager::EntityID id)
{
	if(!componentMasks.contains(id))
	{
		spdlog::error("Magmatic: Cannot destroy entity. Entity does not exist!");
		throw std::out_of_range("Entity does not exist");
	}
	componentMasks.erase(id);
	free_IDs.push(id);
}

bool magmatic::ecs::EntityManager::entityExists(EntityID id) const noexcept
{
	return componentMasks.contains(id);
}

void magmatic::ecs::EntityManager::setComponentMask(EntityID id, ComponentsMask mask)
{
	if(!entityExists(id))
	{
		spdlog::error("Magmatic Cannot update mask. Entity does not exist");
		throw std::runtime_error("Entity does not exist");
	}
	componentMasks[id] = mask;
}

const magmatic::ecs::EntityManager::ComponentsMask &
magmatic::ecs::EntityManager::getComponentMask(EntityID id) const
{
	try
	{
		const ComponentsMask& mask = componentMasks.at(id);
		return mask;
	}
	catch (const std::out_of_range&)
	{
		spdlog::error("Magmatic Cannot get mask. Entity does not exist");
		throw std::runtime_error("Entity does not exist");
	}
}

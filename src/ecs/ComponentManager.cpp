#include "ecs/ComponentManager.hpp"

void magmatic::ecs::ComponentManager::removeEntityComponents(ComponentManager::EntityID id) noexcept
{
	for(auto& [name, mapping] : mappings)
	{
		mapping->remove(id);
	}
}
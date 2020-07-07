#include "ecs/ComponentManager.hpp"

void magmatic::ecs::ComponentManager::removeEntityComponents(ComponentManager::EntityID id) noexcept
{
	for(auto& [mapping_id, mapping_entry] : mappings)
	{
		mapping_entry.mapping->remove(id);
	}
}
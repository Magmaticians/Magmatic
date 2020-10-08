#include "ecs/ComponentManager.hpp"

void magmatic::ecs::ComponentManager::removeEntityComponents(entity_id_t id) noexcept
{
	for(auto& [mapping_id, mapping_entry] : mappings_)
	{
		mapping_entry.mapping->remove(id);
	}
}
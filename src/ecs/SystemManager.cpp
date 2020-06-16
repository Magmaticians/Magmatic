#include "ecs/SystemManager.hpp"

void magmatic::ecs::SystemManager::entityRemoved(SystemManager::EntityID id)
{
	for(auto& entry: systems)
	{
		auto& system = entry.second.system;
		system->handled_entities.erase(id);
	}
}

void magmatic::ecs::SystemManager::updateEntityMask(SystemManager::EntityID id, const SystemManager::ComponentsMask &mask)
{
	for(auto& entry: systems)
	{
		auto& system = entry.second.system;
		const auto& system_mask = entry.second.mask;
		if((system_mask & mask) == system_mask)
		{
			system->handled_entities.insert(id);
		}
		else
		{
			system->handled_entities.erase(id);
		}
	}
}

std::vector<std::string> magmatic::ecs::SystemManager::getAllSystemsID() const
{
	std::vector<std::string> ids;
	std::transform(systems.begin(), systems.end(), std::back_inserter(ids),
		[&](auto entry)
		{
			return entry.first;
		}
	);
	return ids;
}

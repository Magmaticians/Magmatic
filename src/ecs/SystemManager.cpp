#include "ecs/SystemManager.hpp"

void magmatic::ecs::SystemManager::entityRemoved(SystemManager::EntityID id)
{
	for(auto& entry: systems_)
	{
		auto& system = entry.second.system;
		system->handled_entities.erase(id);
	}
}

void magmatic::ecs::SystemManager::updateEntityMask(SystemManager::EntityID id, const SystemManager::ComponentsMask &mask)
{
	for(auto& entry: systems_)
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
	std::transform(systems_.begin(), systems_.end(), std::back_inserter(ids),
	               [&](auto entry)
		{
			return entry.first;
		}
	);
	return ids;
}

magmatic::ecs::SystemManager::SystemManager(
		magmatic::ecs::ComponentManager& component_manager, magmatic::ecs::EventRelay& event_relay
)
: component_manager_(component_manager), event_relay_(event_relay)
{

}

void magmatic::ecs::SystemManager::update(const std::chrono::duration<int64_t, std::micro>& delta)
{
	for(const auto& system_entry: systems_)
	{
		auto& system = system_entry.second.system;
		system->update(delta, component_manager_);
	}
}
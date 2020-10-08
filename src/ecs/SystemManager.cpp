#include "ecs/SystemManager.hpp"
#include "ecs/ECS.hpp"

magmatic::ecs::SystemManager::SystemManager(ECS& ecs, thread_pool_ptr thread_pool) noexcept
		: ecs_(ecs), executor_(std::move(thread_pool), ecs)
{
}

void magmatic::ecs::SystemManager::entityRemoved(entity_id_t id)
{
	for(auto& entry: systems_)
	{
		auto& system_entry = entry.second;
		system_entry.handled_entities.erase(id);
	}
}

void magmatic::ecs::SystemManager::updateEntityMask(entity_id_t id, const components_mask_t &mask)
{
	for(auto& entry: systems_)
	{
		auto& system_entry = entry.second;
		const auto& system_mask = system_entry.components_access_mask.read;

		if((system_mask & mask) == system_mask)
		{
			system_entry.handled_entities.insert(id);
		}
		else
		{
			system_entry.handled_entities.erase(id);
		}
	}
}

std::vector<magmatic::ecs::SystemManager::system_id_t> magmatic::ecs::SystemManager::getAllSystemsId() const
{
	std::vector<system_id_t> ids;
	std::transform(
		systems_.begin(), systems_.end(), std::back_inserter(ids),
		[&](auto entry)
		{
			return entry.first;
		}
	);
	return ids;
}

void magmatic::ecs::SystemManager::update(const delta_time_t& delta)
{
	for(const auto& system_entry: systems_)
	{
		auto& system = system_entry.second.system;
		system->run(delta);
	}
}

void magmatic::ecs::SystemManager::configureSystem(const std::shared_ptr<System>& system)
{
	system->configure(ecs_.eventRelay());
}
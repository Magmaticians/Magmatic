#ifndef MAGMATIC_SYSTEMMANAGER_HPP
#define MAGMATIC_SYSTEMMANAGER_HPP

#include "System.hpp"
#include <memory>
#include <cassert>
#include <chrono>
#include <concepts>



namespace magmatic::ecs
{
	class SystemManager
	{
	public:
		using ComponentsMask = EntityManager::ComponentsMask;
		using EntityID = EntityManager::EntityID;

		template<typename T>
		void registerSystem();

		template<typename T>
		std::shared_ptr<T> getSystem();

		std::vector<std::string> getAllSystemsID() const;

		template<typename T>
		ComponentsMask setSystemMask(const ComponentsMask& mask);

		void entityRemoved(EntityID id);

		void updateEntityMask(EntityID id, const ComponentsMask& mask);

		void update(
				const std::chrono::duration<int64_t, std::micro>& delta,
				const ComponentManager& component_manager
				);

	private:
		struct SystemEntry
		{
			std::shared_ptr<System> system;
			ComponentsMask mask;
		};

		std::unordered_map<std::string, SystemEntry> systems;
	};

	template<typename T>
	requires std::is_default_constructible<T>
	void SystemManager::registerSystem()
	{
		const auto name = typeid(T).name();
		assert(!systems.contains(name));

		SystemEntry entry = {};
		entry.system = std::make_shared<T>();

		systems.insert({name, entry});
	}

	template<typename T>
	std::shared_ptr<T> SystemManager::getSystem()
	{
		const auto name = typeid(T).name();

		return std::dynamic_pointer_cast<T>(systems.at(name).system);
	}

	template<typename T>
	SystemManager::ComponentsMask SystemManager::setSystemMask(const SystemManager::ComponentsMask &mask)
	{
		const auto name = typeid(T).name();
		auto& system_entry = systems.at(name);

		const auto old_mask = system_entry.mask;
		system_entry.mask = mask;

		return old_mask;
 	}
}

#endif //MAGMATIC_SYSTEMMANAGER_HPP

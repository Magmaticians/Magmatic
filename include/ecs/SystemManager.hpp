#ifndef MAGMATIC_SYSTEMMANAGER_HPP
#define MAGMATIC_SYSTEMMANAGER_HPP

#include "System.hpp"
#include "EventRelay.hpp"
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

		SystemManager(ComponentManager& component_manager, EventRelay& event_relay);

		template<typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		void registerSystem(Args&&... args);

		template<typename T>
		std::shared_ptr<T> getSystem();

		std::vector<std::string> getAllSystemsID() const;

		template<typename T>
		ComponentsMask setSystemMask(const ComponentsMask& mask);

		void entityRemoved(EntityID id);

		void updateEntityMask(EntityID id, const ComponentsMask& mask);

		void update(const std::chrono::duration<int64_t, std::micro>& delta);

	private:
		struct SystemEntry
		{
			std::shared_ptr<System> system;
			ComponentsMask mask;
		};

		ComponentManager& component_manager_;
		EventRelay& event_relay_;

		std::unordered_map<std::string, SystemEntry> systems_;
	};

	template<typename T>
	std::shared_ptr<T> SystemManager::getSystem()
	{
		const auto name = typeid(T).name();

		return std::dynamic_pointer_cast<T>(systems_.at(name).system);
	}

	template<typename T>
	SystemManager::ComponentsMask SystemManager::setSystemMask(const SystemManager::ComponentsMask &mask)
	{
		const auto name = typeid(T).name();
		auto& system_entry = systems_.at(name);

		const auto old_mask = system_entry.mask;
		system_entry.mask = mask;

		return old_mask;
 	}

	template<typename T, typename... Args>
	requires std::constructible_from<T, Args...>
	void SystemManager::registerSystem(Args &&... args)
	{
		const auto name = typeid(T).name();
		assert(!systems_.contains(name));

		SystemEntry entry = {};
		entry.system = std::make_shared<T>(std::forward<Args>(args)...);

		entry.system->configure(event_relay_);

		systems_.insert({name, entry});
	}
}

#endif //MAGMATIC_SYSTEMMANAGER_HPP

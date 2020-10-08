#ifndef MAGMATIC_SYSTEMMANAGER_HPP
#define MAGMATIC_SYSTEMMANAGER_HPP

#include "System.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "ECSTypes.hpp"
#include "executor/ParallelExecutor.hpp"
#include <memory>
#include <cassert>
#include <chrono>
#include <typeindex>
#include <concepts>


namespace magmatic::ecs
{
	//forward declaration ECS
	class ECS;

	template<typename T>
	concept ECSSystem = std::derived_from<T, System>;

	class SystemManager
	{
	private:
		class SystemSettings;
	public:
		using component_type_id_t = ComponentManager::component_type_id_t;
		using system_id_t = std::type_index;
		using executor_t = executor::ParallelExecutor;

		SystemManager(ECS& ecs, thread_pool_ptr thread_pool) noexcept;

		template<ECSSystem T, typename... Args>
		requires std::constructible_from<T, Args...>
		SystemSettings registerSystem(Args&&... args);

		template<ECSSystem T>
		std::shared_ptr<T> getSystem();

		std::vector<system_id_t> getAllSystemsId() const;

		template<ECSSystem T>
		components_mask_t setSystemReadMask(const components_mask_t& mask);
		template<ECSSystem T>
		components_mask_t setSystemWriteMask(const components_mask_t& mask);

		template<ECSSystem T>
		void setSystemReadComponent(component_type_id_t component_type_id);
		template<ECSSystem T>
		void setSystemWriteComponent(component_type_id_t component_type_id);

		void entityRemoved(entity_id_t id);

		void updateEntityMask(entity_id_t id, const components_mask_t& mask);

		void update(const delta_time_t& delta);

	private:
		void setSystemReadComponent(system_id_t system_id, component_type_id_t component_type_id)
		{
			auto& system_entry = systems_.at(system_id);
			system_entry.components_access_mask.read.set(component_type_id);
		}
		void setSystemWriteComponent(system_id_t system_id, component_type_id_t component_type_id)
		{
			auto& system_entry = systems_.at(system_id);
			system_entry.components_access_mask.write.set(component_type_id);
		}

		struct AccessMask
		{
			components_mask_t read;
			components_mask_t write;
		};

		struct SystemEntry
		{
			std::shared_ptr<System> system;
			AccessMask components_access_mask;

			std::set<entity_id_t> handled_entities;
		};

		class SystemSettings
		{
		public:

			SystemSettings(system_id_t id, SystemManager& manager)
			:id_(id), manager_(manager)
			{};

			SystemSettings& read(component_type_id_t component_type_id)
			{
				manager_.setSystemReadComponent(id_, component_type_id);
				return *this;
			};

			SystemSettings& write(component_type_id_t component_type_id)
			{
				manager_.setSystemWriteComponent(id_, component_type_id);
				return *this;
			};

		private:
			system_id_t id_;
			SystemManager& manager_;
		};

		ECS& ecs_;
		executor_t executor_;

		std::unordered_map<system_id_t, SystemEntry> systems_;

		void configureSystem(const std::shared_ptr<System>& system);
	};

	template<ECSSystem T>
	std::shared_ptr<T> SystemManager::getSystem()
	{
		const auto system_type_index = std::type_index(typeid(T));
		return std::dynamic_pointer_cast<T>(systems_.at(system_type_index).system);
	}

	template<ECSSystem T>
	components_mask_t SystemManager::setSystemReadMask(const components_mask_t &mask)
	{
		const auto system_type_index = std::type_index(typeid(T));
		auto& system_entry = systems_.at(system_type_index);

		const auto old_mask = system_entry.components_access_mask.read;
		system_entry.components_access_mask.read = mask;

		return old_mask;
 	}

 	template<ECSSystem T>
	components_mask_t SystemManager::setSystemWriteMask(const components_mask_t &mask)
	{
		const auto system_type_index = std::type_index(typeid(T));
		auto& system_entry = systems_.at(system_type_index);

		const auto old_mask = system_entry.components_access_mask.write;
		system_entry.components_access_mask.write = mask;

		return old_mask;
 	}

	template<ECSSystem T>
	void SystemManager::setSystemReadComponent(SystemManager::component_type_id_t component_type_id)
	{
		const auto system_type_index = std::type_index(typeid(T));
		auto& system_entry = systems_.at(system_type_index);

		system_entry.components_access_mask.read.set(component_type_id);
	}

	template<ECSSystem T>
	void SystemManager::setSystemWriteComponent(SystemManager::component_type_id_t component_type_id)
	{
		const auto system_type_index = std::type_index(typeid(T));
		auto& system_entry = systems_.at(system_type_index);

		system_entry.components_access_mask.write.set(component_type_id);
	}

	template<ECSSystem T, typename... Args>
	requires std::constructible_from<T, Args...>
	SystemManager::SystemSettings SystemManager::registerSystem(Args &&... args)
	{
		const auto system_type_index = std::type_index(typeid(T));
		assert(!systems_.contains(system_type_index));

		SystemEntry entry = {};
		entry.system = std::make_shared<T>(std::forward<Args>(args)...);

		configureSystem(entry.system);

		systems_.insert({system_type_index, entry});

		return SystemSettings(system_type_index, *this);
	}
}

#endif //MAGMATIC_SYSTEMMANAGER_HPP
